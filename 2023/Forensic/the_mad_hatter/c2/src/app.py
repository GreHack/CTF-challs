from flask import Flask, jsonify, request, redirect, send_from_directory
from flask_basicauth import BasicAuth

# Flask config
app = Flask(__name__, static_url_path='/static')
app.config["SECRET_KEY"] = '8f42a73054b1749f8f58848be5e6502c'
app.config['BASIC_AUTH_USERNAME'] = 'c2_m4st3r'
app.config['BASIC_AUTH_PASSWORD'] = '29a3675bc87ad32852f7935741f8e98cee547c65'

basic_auth = BasicAuth(app)

# Redirect to dummy
@app.route('/', methods=['GET'])
def index():
    return redirect('https://grehack.fr')

# Provides key and IV if auth success
@app.route('/4af5da2e9ef5efd3520c9a9f463dbdee', methods=['GET'])
@basic_auth.required
def get_key_iv():

    key = "4a448c0831b578470664af35a7067315"
    iv = "1337733113377331"

    data = {
        "key": key,
        "iv": iv
    }

    print(f"Delivred key : {key} and IV : {iv}")

    return jsonify(data)

if __name__ == '__main__':
    app.run(port=5000, host='0.0.0.0', debug=False)