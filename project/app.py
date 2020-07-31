import os

from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from flask_login import LoginManager
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

# from helpers import apology, lookup

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
# db = SQL("sqlite:///robots.db")

@app.route("/")
def index():
    """Homepage of robot manager"""

    if session['logged_in'] == False:
        return redirect("login")
    # robots = db.execute("SELECT * FROM robots")

    # Redirect user to home page
    return render_template("index.html", robots=robots)

# def index():
#     """Show portfolio of stocks"""
#     user_cash = db.execute("SELECT cash FROM users WHERE id=:username", username=session["user_id"])
#     cash = user_cash[0]["cash"]

#     stocks = db.execute("SELECT symbol, SUM(shares) AS total_shares FROM transactions WHERE user_id=:username GROUP BY symbol", username=session["user_id"])

#     total = cash
#     for stock in stocks:
#         quote = lookup(stock["symbol"])
#         stock.update(quote)
#         total+= stock["price"] * stock["total_shares"]

#     # Redirect user to home page
#     return render_template("index.html", stocks=stocks, cash=cash, total=total)

@app.route("/add_robot", methods=["GET", "POST"])
def add_robot():
    """Adds a new robot to the server"""
    if request.method == "POST":

        if not request.form.get("password"):
            return apology("missing password", 400)

        if not request.form.get("confirmation"):
            return apology("must provide password", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("add_robot.html")


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
        # rows = db.execute("SELECT * FROM users WHERE username = :username",
        #                   username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        session['logged_in'] = True

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    session['logged_in'] = False
    return redirect("login")

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
        # db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), pwhash)

        return redirect("/")
    # User reached route via GET
    else:
        return render_template("register.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
