import os
import sqlite3
from sqlite3 import Error

from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from flask_login import LoginManager
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology

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

# Create database connection to the SQLite database
conn = None
try:
    conn = sqlite3.connect("robots.db", check_same_thread=False)
except Error as e:
    print(e)

@app.route("/")
def index():
    """Homepage of robot manager"""

    if not session:
        return redirect("/login")
    cur = conn.cursor()
    robots = cur.execute("SELECT * FROM robots")

    # Redirect user to home page
    return render_template("index.html", robots=robots)

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
        cur = conn.cursor()
        cur.execute("SELECT * FROM users WHERE username=:username", {"username": request.form.get("username")})

        rows = cur.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][3], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0][0]
        session["username"] = rows[0][1]
        session["name"] = rows[0][2]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    session.clear()
    return redirect("login")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()
    if request.method == "POST":
        username = request.form.get("username")
        name = username.capitalize()

        if request.form.get("name"):
            name = request.form.get("name").capitalize()

        pwhash = generate_password_hash(request.form.get("password"))

        # Insert username, name and password hash value into database
        cur = conn.cursor()

        # Check if username already exists in database
        cur.execute("SELECT * FROM users WHERE username=:username", {"username": username})
        rows = cur.fetchall()
        
        if len(rows) > 0:
            return apology("Username Already Exists", 400)

        # Commit new user into database
        cur.execute("INSERT INTO users (username, name, hash) VALUES(?, ?, ?)", (username, name, pwhash))
        cur.execute("SELECT * FROM users WHERE username=:username", {"username": username})
        rows = cur.fetchall()
        conn.commit()

        # Remember which user and log in
        session["user_id"] = rows[0][0]
        session["username"] = username
        session["name"] = name

        return redirect("/")
    # User reached route via GET
    else:
        return render_template("register.html")

@app.route("/forgot_password", methods=["GET", "POST"])
def forgot_password():
    """Log user in"""

    # Forget any user_id
    session.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Query database for 1st user account in database (admin account)
        cur = conn.cursor()
        cur.execute("SELECT * FROM users WHERE id=:id", {"id": "1"})
        rows = cur.fetchall()

        # Ensure admin account exists and password of admin account is correct
        if len(rows) == 1 and check_password_hash(rows[0][3], request.form.get("password")):
            session["username"] = rows[0][1]
            # Redirect user to home page
            return redirect("/change_password")

        else:
            return redirect("/forgot_password")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("forgot_password.html")

@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    """Allow users to change password"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Set cursor for database
        cur = conn.cursor()

        # If user is changing password from forgot_password route
        if "username" not in session:
            username = request.form.get("username")

            # Check if username exists in database
            cur.execute("SELECT * FROM users WHERE username=:username", {"username": username})
            rows = cur.fetchall()
            if len(rows) == 0:
                return apology("User Does Not Exist", 400)
        else:
            # Check if username already exists in database
            cur.execute("SELECT * FROM users WHERE username=:username", {"username": session["username"]})
            rows = cur.fetchall()
            if not check_password_hash(rows[0][3], request.form.get("old_password")):
                return apology("Wrong Password", 400)
        username = rows[0][1]

        # Get new password from the user
        new_pwhash = generate_password_hash(request.form.get("password"))

        # Update database with new password
        cur.execute("UPDATE users SET hash = ? WHERE username = ?", (new_pwhash, username))
        cur.execute("SELECT * FROM users WHERE username=:username", {"username": username})
        rows = cur.fetchall()
        conn.commit()

        # Remember and login user
        session["user_id"] = rows[0][0]
        session["username"] = rows[0][1]
        session["name"] = rows[0][2]

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # If user is changing password from forgot_password route
        if "username" not in session:
            # if session["username"] != rows[0][1]:
            return render_template("forgot_password.html")
        else:
            # Query database for 1st user account in database (admin account)
            cur = conn.cursor()
            cur.execute("SELECT * FROM users WHERE id=:id", {"id": "1"})
            rows = cur.fetchall()
            # Forget admin user login
            if session["username"] == rows[0][1] and "user_id" not in session:
                session.clear()
            return render_template("change_password.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
