from flask import (
    Flask, 
    request, 
    render_template, 
    jsonify
)   
from api.queries import * 
from api.Secret import * 
import os
import jwt
import urllib

app = Flask(__name__)

SECRET = os.urandom(24)
ENDPOINTS = ["users"] # add new endpoints in the futur..

@app.route("/")
def home():
    return render_template("index.html")

# To manage multi-endpoints
@app.route("/api/<endpoint>/<id_user>")
def parse(endpoint, id_user):
    if endpoint.lower() in ENDPOINTS:
        if endpoint == "users":
            return jsonify(error="This endpoint is for admins only."), 403
        return jsonify(get_user(int(id_user)))
    else:
        return jsonify(error="This page does not exists."), 404

@app.route("/api/password-reset", methods=["POST"])
def password_reset():
    json = request.get_json()
    try:
        token = json["token"]
        password = json["password"]
        if update_password(token, password):
            return jsonify(success="The password has been reset.")
        else:
            return jsonify(error="An error has occured.")
    except Exception as e:
        print(e)
        return jsonify(error="Parameter 'token' or 'password' are missing.")

@app.route("/api/login", methods=["POST"])
def login():
    json = request.get_json()
    try:
        datas = api_login(json["username"], json["password"])
        jwt_token = jwt.encode(datas, SECRET, algorithm="HS256")
        return jsonify(jwt=jwt_token)
    except Exception as e:
        print(e)
        return jsonify(error="Incorrect username or password.")

@app.route("/api/admin", methods=["POST"])
def admin():
    jwt_token = request.headers.get("X-Api-Key")
    if jwt_token is None:
        return jsonify(error="You must provide X-Api-Key header.")
    try:
        if jwt.decode(jwt_token, SECRET, algorithms=["HS256"])["role"] == "ADMIN":
            secret = request.get_json()["secret"]
            secret = Secret(secret)
            print(secret)
            return render_template("secret.html", secret=f"{secret}".format(secret=secret))
        else:
            return jsonify(error="You must be admin !")
    except Exception as e:
        return jsonify(error=f"An eror has occured : {e}")

@app.route(f"/api/{SECRET_ENDPOINT}", methods=["POST"])
def secret():
    jwt_token = request.headers.get("X-Api-Key")
    if jwt_token is None:
        return jsonify(error="You must provide X-Api-Key header.")
    try:
        if jwt.decode(jwt_token, SECRET, algorithms=["HS256"])["role"] == "ADMIN":
            filename = urllib.parse.unquote(request.get_json()['filename'])
            data = "This file doesn't exist"
            bad_chars = ["../", "\\", "."]
            is_safe = all(char not in filename for char in bad_chars)
            
            if is_safe:
                filename = urllib.parse.unquote(filename)
                if os.path.isfile('./'+ filename):
                    with open(filename) as f:
                        data = f.read()
            return jsonify(data)
        else:
            return jsonify(error="You must be admin !")
    except Exception as e:
        return jsonify(error=f"An eror has occured : {e}")