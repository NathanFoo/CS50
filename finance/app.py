import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd" ] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    # get user id
    user_id = session["user_id"]

    # get user stocks data
    stocks = db.execute(
        "SELECT * "
        "FROM stocks "
        "WHERE user_id = ?",
        user_id
    )

    # initialize total net variable
    total = 0

    # Add price, holding price, name, and total net of stocks from lookup
    for stock in stocks:
        response = lookup(stock["symbol"])
        stock["price"] = response["price"]
        stock["holding"] = response["price"] * stock["shares"]
        stock["name"] = response["name"]
        total += stock["holding"]

    # get user cash from sql finance.db
    user = db.execute(
        "SELECT cash "
        "FROM users "
        "WHERE id = ?"
        user_id
    )

    # get user balance
    balance = user[0]["cash"]

    # add balance to user total
    total += balance

    return render_template("index.html", stocks=stocks, balance=usd(balance), total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # Ensures symbol entered
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 400)
        # Checks if symbol valid
        response = lookup(symbol)
        if not response:
            return apology("not valid symbol", 400)
        # Ensures shares entered
        shares = request.form.get("shares")
        if not shares:
            return apology("must provide a share count", 400)

        if not shares.isnumeric():
            return apology("must provide a positive integer", 400)

        shares = int(shares)
        if shares < 1:
            return apology("share must be positive integer", 400)

        # Get sessions user_id
        user_id = session["user_id"]

        # Get user balance
        user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        userBalance = user[0]["cash"]

        # Get total cost
        cost = response["price"] * shares

        # Ensure user balance is sufficient for purchase
        newBalance = userBalance - cost
        if newBalance < 0:
            return apology("Insufficient Funds", 403)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", newBalance, user_id)

        # Get date
        date = datetime.datetime.now()

        stocks = db.execute(
            "INSERT INTO stocks (user_id, symbol, shares) "
            "VALUES (?,?,?) "
            "ON CONFLICT(symbol) DO UPDATE "
            "SET shares = shares + ? ",
            user_id, symbol, shares, shares
        )

        # Populate purchases table with purchase data
        db.execute(
            "INSERT INTO purchases (user_id, symbol, purchases_price,"
                "shares_bought, year, month, day, hour, minute)"
            "VALUES (?,?,?,?,?,?,?,?,?)",
            user_id, symbol, cost, shares, date.year,
            date.month, date.day, date.hour, date.minute
        )

        return redirect("/")

    return render_template("buy.html", symbol=None,
                           name=None, price=None)


@app.route("/history")
@login_required
def history():

        # Get sessions user_id
        user_id = session["user_id"]

        # Get purchase history
        purchases = db.execute(
            "SELECT * "
            "FROM purchases "
            "WHERE user_id = ?",
            user_id
        )

        # Get sells history
        sells = db.execute(
            "SELECT * "
            "FROM sells "
            "WHERE user_id = ?",
            user_id
        )

        return render_template("history.html", purchases=purchases, sells=sells)

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
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote(): # Takes response
    if request.method == "POST":
        # Ensures symbol was entered
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 400)
        # Checks if symbol is valid
        response = lookup(symbol)
        if not response:
            return apology("not valid symbol", 400)
        else:
            return render_template(
                "quote.html", symbol=response["symbol"],
                name=response["name"], price=usd(response["price"]))
    else:
        return render_template("quote.html", symbol=None,
                               name=None, price=None)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        password = request.form.get("password")
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        confirmPassword = request.form.get("confirmation")
        if not confirmPassword:
            return apology("must confirm password", 400)

        # Ensure passwords match
        if not password == confirmPassword:
            return apology("passwords do not match", 400)

        # generate password hash
        h = generate_password_hash(password, method='scrypt', salt_length=16)

        # attempt sql prompt to insert new user
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, h)
        except ValueError: # will fail as username is unique (no username copies)
            return apology("username already in use", 400)

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":

        # Ensures symbol entered
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 403)

        # Checks if symbol valid
        response = lookup(symbol)
        if not response:
            return apology("not valid symbol", 403)

        # Ensures shares entered
        shares = request.form.get("shares")
        if not shares:
            return apology("must provide a share count", 403)

        # Ensures shares is a positive integer
        shares = int(shares)
        if shares < 1:
            return apology("share must be positive integer", 403)

        # Get sessions user_id
        user_id = session["user_id"]

        # Get total cost
        cost = response["price"] * shares

        # Get date
        date = datetime.datetime.now()

        stocks = db.execute(
            "SELECT shares "
            "FROM stocks "
            "WHERE user_id = ? "
            "AND symbol = ? ",
            user_id, symbol
        )

        # Check if user owns any shares
        if not stocks:
            apology("Do not currently own any shares", 400)

        result = stocks[0]["shares"] - shares

        print(f"\n\n{result}\n\n")

        # update owned shares if owned shares still greater than 1
        if result > 0:
            db.execute(
                "UPDATE stocks "
                "SET shares = shares - ? "
                "WHERE user_id = ? "
                "AND symbol = ?",
                shares, user_id, symbol
            )
        # remove row if sold all shares
        elif result == 0:
            db.execute(
                "DELETE FROM stocks "
                "WHERE user_id = ? "
                "AND symbol = ? ",
                user_id, symbol
            )
        # error if try
        else:
            return apology("Insufficient shares", 400)

        # Populate sells table with sell data
        db.execute(
            "INSERT INTO sells (user_id, symbol, sell_price,"
                "shares_sold, year, month, day, hour, minute)"
            "VALUES (?,?,?,?,?,?,?,?,?)",
            user_id, symbol, cost, shares, date.year,
            date.month, date.day, date.hour, date.minute
        )

        # Add money to balance
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", response["price"] * shares, user_id)

        # return to index.html main hud
        return redirect("/")

    elif request.method == "GET":

        # Get sessions user_id
        user_id = session["user_id"]

        # Get stocks owned
        stocks = db.execute(
                "SELECT symbol "
                "FROM stocks "
                "WHERE user_id = ? ",
                user_id
        )

        return render_template("sell.html", stocks=stocks)

    return render_template("sell.html")
