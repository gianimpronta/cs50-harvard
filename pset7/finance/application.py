from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Render Home page"""
    # get user id from session
    user_id = session.get("user_id")

    # ge user balance
    user_balance = round(float(db.execute("SELECT cash FROM users WHERE id = :id",
        id = user_id)[0]['cash']), 2)

    # get user stocks
    rows = db.execute("SELECT stock AS name, sum(shares) AS shares \
                       FROM transactions \
                       GROUP BY stock \
                       HAVING user_id = :id AND shares > 0", id = user_id)

    # initialize user total
    total_stocks = 0

    # iterate over every user's stocks
    for row in rows:

        # get quote for stock
        row["quote"] = round(lookup(row["name"])["price"], 2)

        # calculate total for each stock
        row["total"] = float(row["quote"]) * float(row["shares"])

        # add to total stocks
        total_stocks += row["total"]

    # round total stocks
    total_stocks = round(total_stocks, 2)

    # render index page passing two variables
    return render_template("index.html", stocks = rows, total = total_stocks,
                            balance = user_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return apology("provide stock symbol, you must.")

        stock_number = request.form.get("stock_number")

        # ensure stock number is positive
        try:
            if float(stock_number) <= 0:
                return apology("provide a positive number, you must.")
            else:
                stock_number = float(stock_number)
        except ValueError:
            return apology("provide the number of shares you want.")

        # get stock price
        stock_lookup = lookup(request.form.get("stock_symbol"))

        # ensure username exists and password is correct
        if not stock_lookup:
            return apology("invalid stock.")
        else:
            stock_price = float(stock_lookup['price'])

        # get user balance
        user_balance = float(db.execute("SELECT cash FROM users WHERE id = :id",
        id = session.get("user_id"))[0]['cash'])

        # get stock symbol
        stock_symbol = request.form.get("stock_symbol").upper()

        # calculate total
        buy_total = float(stock_price * stock_number)

        # ensure that user can buy those stocks
        if buy_total > user_balance:
            return apology("You have not enough money.")
        else:
            # insert transaction into database
            db.execute("INSERT INTO transactions \
            ('user_id', 'shares', 'stock', 'price', 'action') \
            VALUES (:userid, :shares, :stock, :price, 'buy')",
                        userid = session.get("user_id"),
                        stock = stock_symbol,
                        price = stock_price,
                        shares = stock_number)
            new_balance = user_balance - buy_total
            db.execute("UPDATE users \
                        SET cash = :newbalance \
                        WHERE id = :userid", newbalance = new_balance,
                        userid = session.get("user_id"))
            flash("Your transaction was successful. You bought \
                {} {} shares at {}".format(stock_number,
                stock_symbol,stock_price))
            return redirect(url_for("buy"))
    else:

        # redirect user to buy page
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    user_id = session.get("user_id")
    rows = db.execute("SELECT action, stock, price, shares, date \
                       FROM transactions \
                       WHERE user_id = :user_id", user_id = user_id)

    # Test if user have stock in account
    if len(rows) == 0:
        flash("You don't have any stocks.")
        return render_template("nothing.html")

    return render_template("history.html", stocks = rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):

            return apology("provide username, you must")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("provide password, you must")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # user feedback
        flash('You were successfully logged in')

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # user feedback
    flash('You were successfully logged out.')

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # lookup for stock quote
        stock = lookup(request.form.get("stock_symbol"))

        # redirect user to quote results
        return render_template("quoted.html", stock=stock)

    else:

        # redirect user to quote form
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):

            return apology("enter a username, you must.")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("provide password, you must.")

        # ensure password confirmation was submitted
        elif not request.form.get("password_confirm"):
            return apology("confirm password, you must.")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                           username=request.form.get("username"))

        # check if username already exists
        if len(rows) != 0 :
            return apology("Hrrmmm. User already exists. Another one choose.")

        # ensure that password and password confirmation are equal
        if request.form.get("password") != request.form.get("password_confirm"):
            return apology("Equal password and password confirmation should be.")

        # create
        db.execute("INSERT INTO 'users' ('username','hash') VALUES (:username, :password)",
                    username = request.form.get("username"),
                    password = pwd_context.hash(request.form.get("password")))

        # user feedback
        flash('You were successfully registered.')

        # redirect user to home page
        return redirect(url_for("login"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # get user id from session
    user_id = session.get("user_id")

    if request.method == "GET":
        # get user stocks
        rows = db.execute("SELECT stock AS name, sum(shares) AS shares \
                           FROM transactions \
                           GROUP BY stock \
                           HAVING user_id = :id AND shares > 0", id = user_id)

        # Test if user have stock in account
        if len(rows) == 0:
            flash("You don't have any stocks.")
            return render_template("nothing.html")

        # iterate over every user's stocks
        for row in rows:

            # get quote for stock
            row["quote"] = round(lookup(row["name"])["price"], 2)

        # render index page passing two variables
        return render_template("sell.html", stocks = rows)

    else:

        stock_name = request.form.get("stock")
        sell_quantity = request.form.get("quantity")
        stock_price = float(lookup(stock_name)['price'])

        # ensure sell_quantity is positive
        try:
            if float(sell_quantity) <= 0:
                return apology("provide a positive number, you must.")
            else:
                sell_quantity = float(sell_quantity)
        except ValueError:
            return apology("provide the number of shares you want to sell.")

        # get all stocks from user
        rows = db.execute("SELECT stock AS name, sum(shares) AS shares \
                           FROM transactions \
                           GROUP BY stock \
                           HAVING user_id = :id AND stock = :stock AND shares > 0",
                           id = user_id, stock = stock_name)

        stock_quantity = float(rows[0]['shares'])

        if sell_quantity > stock_quantity:
            return apology("you have not enough stocks")
        else:
            user_balance = float(db.execute("SELECT cash FROM users WHERE id = :id",
                                             id = session.get("user_id"))[0]['cash'])
            sell_total = sell_quantity * stock_price
            # insert transaction into database
            db.execute("INSERT INTO transactions \
            ('user_id', 'shares', 'stock', 'price', 'action') \
            VALUES (:userid, :shares, :stock, :price, 'sell')",
                        userid = user_id,
                        stock = stock_name,
                        price = stock_price,
                        shares = sell_quantity*(-1))
            new_balance = user_balance + sell_total
            db.execute("UPDATE users \
                        SET cash = :newbalance \
                        WHERE id = :userid", newbalance = new_balance,
                        userid = user_id)
            if sell_quantity > 1:
                flash("You sold {} {} shares".format(sell_quantity, stock_name))
            else:
                flash("You sold {} {} share".format(sell_quantity, stock_name))
            return redirect(url_for("sell"))


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    if request.method == "GET":
        return render_template("password.html")

    else:
        old_pass = request.form.get("oldpassword")
        new_pass = request.form.get("newpassword")
        new_pass_conf = request.form.get("newpassword_confirm")
        user_id = session.get("user_id")
        # query database for username
        rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id = user_id)

        # ensure username exists and password is correct
        if not pwd_context.verify(old_pass, rows[0]["hash"]):
            return apology("invalid password")
        else:
            db.execute("UPDATE users\
                       SET hash = :hash\
                       WHERE id = :user_id", user_id = user_id,
                       hash = pwd_context.hash(new_pass))
            flash("You sucessfully changed your password.")
            return redirect(url_for("index"))