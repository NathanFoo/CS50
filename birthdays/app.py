import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

months = [ "January", "February", "March", "April", "May", "June",
           "July", "August", "September", "October", "November", "December"]

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        day = request.form.get("day")
        month = request.form.get("months")
        if (not name):
            name = "Empty"
        if (not day):
            day = 1
        if (not month):
            month = 1
        db.execute("INSERT INTO birthdays (name, day, month) VALUES(?, ?, ?)", name, day, month)
        return redirect("/")
    else:
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays, months=months)

@app.route("/<int:id>", methods=["GET", "POST"])
def delete(id):
    db.execute("DELETE FROM birthdays WHERE id=?", id)
    return redirect("/")
