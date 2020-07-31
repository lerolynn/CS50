import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

#export API_KEY=pk_b1c01d00572941f197ff83a71b8e6e75
# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_cash = db.execute("SELECT cash FROM users WHERE id=:username", username=session["user_id"])
    cash = user_cash[0]["cash"]

    stocks = db.execute("SELECT symbol, SUM(shares) AS total_shares FROM transactions WHERE user_id=:username GROUP BY symbol", username=session["user_id"])

    total = cash
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock.update(quote)
        total+= stock["price"] * stock["total_shares"]

    # Redirect user to home page
    return render_template("index.html", stocks=stocks, cash=cash, total=total)

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow user to change password"""
    if request.method == "POST":

        # Check that password and new passwords are input
        if not request.form.get("password"):
            return apology("must provide old password", 400)

        if not request.form.get("new_password"):
            return apology("must provide new password", 400)

        if not request.form.get("confirmation"):
            return apology("must confirm new password", 400)

        # Check that new passwords match
        if request.form.get("new_password") != request.form.get("confirmation"):
            return apology("new passwords must match", 400)

        # Generate password hash from database
        dbhash = db.execute("SELECT hash FROM users WHERE id=:username", username=session["user_id"])

        # Check that old password matches password hash from database
        if not check_password_hash(dbhash[0]["hash"], request.form.get("password")):
            return apology("wrong password provided", 400)

        # Generate hash from new password
        new_hash = generate_password_hash(request.form.get("new_password"))

        # Update users table with new password hash
        db.execute("UPDATE users SET hash=:new_hash WHERE id=:user_id", new_hash=new_hash, user_id=session["user_id"])

        return redirect("/")

    return render_template("change_password.html")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Check that user provides number
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Check that user provides number of shares
        if not request.form.get("number"):
            return apology("must provide number", 403)

        # Get share details and number of shares requested
        quote = lookup(request.form.get("symbol"))
        number = int(request.form.get("number"))

        # Check that stock requested exists
        if quote == None:
            return apology("missing shares", 400)

        # Check that number of shares bought is not < 1
        if number <= 0:
            return apology("number of shares must be > 0", 400)

        # Get amount of cash the user has
        cash = db.execute("SELECT cash FROM users WHERE id=:username", username=session["user_id"])

        # Check that user can afford stock
        if quote["price"] * number > cash[0]["cash"]:
            return apology("can't afford", 400)

        # Update database with new transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, share_price) VALUES (?, ?, ?, ?)",
                   session["user_id"], request.form.get("symbol").upper(), number, quote["price"])

        # Update amount of cash the user has
        cash_left = cash[0]["cash"] - quote["price"] * number
        db.execute("UPDATE users SET cash=:cash_left WHERE id=:user_id", cash_left=cash_left, user_id=session["user_id"])

        # Redirect user to homepage
        return redirect("/")
    else:

        # Direct user to buy page
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT symbol, shares, share_price, datetime FROM transactions WHERE user_id=:username ORDER BY datetime", username=session["user_id"])
    print(transactions)

    # Redirect user to home page
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        # Check that the user provided a symbol
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        else:
            quote = lookup(request.form.get("symbol"))

            # Check that the provided stock symbol exists
            if quote == None:
                return apology("invalid symbol", 400)
            else:
                # Return stock price page
                return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()
    if request.method == "POST":

        # Check that passwords are input and are identical
        if not request.form.get("password"):
            return apology("missing password", 400)

        if not request.form.get("confirmation"):
            return apology("must provide password", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Generate password hash from password
        pwhash = generate_password_hash(request.form.get("password"))

        # Insert username, password has and cash value into database
        db.execute("INSERT INTO users (username, hash, cash) VALUES(?, ?, 10000)", request.form.get("username"), pwhash)

        return redirect("/")
    # User reached route via GET
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get symbols available to display
    symbols = db.execute("SELECT DISTINCT symbol FROM transactions WHERE user_id=:user_id", user_id=session["user_id"])

    if request.method == "POST":

        # Require user to provide symbol
        if not request.form.get("symbol"):
            return apology("must provide symbols", 400)

        # Requires user to provide number
        if not request.form.get("number"):
            return apology("must provide number of shares", 400)

        # Get share symbol and number of shares to be sold from the user
        symbol = request.form.get("symbol")
        number = int(request.form.get("number"))

        # Check that number of shares sold is not < 1
        if number <= 0:
            return apology("number of shares must be > 0", 400)

        # Get number of shares user has from database
        total_shares = db.execute("SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id=:username AND symbol=:symbol", username=session["user_id"], symbol=symbol)

        # Check that the user has enough shares to sell
        if total_shares[0]["total_shares"] < number:
            return apology("too many shares", 400)

        # Lookup share details at time of transaction
        quote = lookup(symbol)

        # Update database with new transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, share_price) VALUES (?, ?, ?, ?)",
                   session["user_id"], request.form.get("symbol").upper(), -number, quote["price"])

        # Get amount of cash the user has
        cash = db.execute("SELECT cash FROM users WHERE id=:username", username=session["user_id"])

        # Update amount of cash the user has
        cash_left = cash[0]["cash"] + quote["price"] * number
        db.execute("UPDATE users SET cash=:cash_left WHERE id=:user_id", cash_left=cash_left, user_id=session["user_id"])
        return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
