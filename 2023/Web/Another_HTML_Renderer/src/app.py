from flask import Flask, request, make_response, render_template, jsonify, redirect
from subprocess import run
from json import loads
from re import match

app = Flask(__name__)

@app.errorhandler(404)
def page_not_found(e):
    return redirect("/render"), 302

@app.route("/render")
def index():
    settings = ""
    try:
        settings = loads(request.cookies.get("settings"))
    except: pass

    if settings:
        res = make_response(render_template("index.html",
            backgroundColor=settings["backgroundColor"] if "backgroundColor" in settings else "#ffde8c",
            textColor=settings["textColor"] if "textColor" in settings else "#000000",
            html=settings["html"] if "html" in settings else ""
        ))
    else:
        res = make_response(render_template("index.html", backgroundColor="#ffde8c", textColor="#000000"))
        res.set_cookie("settings", "{}")

    return res

@app.route("/report")
def report():
    url = request.args.get("url")
    if not url:
        return jsonify({ "res": "\"url\" GET parameter must be use!" })

    if not match("^http(s)?://.*", url):
        return jsonify({ "res": "The URL must match ^http(?)://.* !" })

    else:
        run(["node", "/usr/app/bot/bot.js", url])
        return jsonify({ "res": "URL sent to the bot!" })

app.run("0.0.0.0", 5000)
