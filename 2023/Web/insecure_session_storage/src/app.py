from flask import Flask, session, request, render_template, jsonify
from cachelib.file import FileSystemCache
from flask_session import Session
from secrets import token_hex
from os.path import join
from pydash import set_

# custom caching mechanism
class Settings:
    def __init__(self):
        pass # TODO

class LocalCache(FileSystemCache):
    def _get_filename(self, key: str) -> str:
        if ".." in key:
            key = token_hex(8)
        return join(self._path, key)

# init
app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)
app.session_interface.cache = LocalCache("flask_session")

# routes
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "GET":
        return render_template("index.html")
    else:
        body = request.json
        if not "key" in body or not "value" in body:
            return jsonify({ "res": "[ERROR] key & value must be set" })
        set_(Settings(), body["key"], body["value"])
        return render_template("index.html")

# main
if __name__ == "__main__":
    app.run("0.0.0.0", 5000)
