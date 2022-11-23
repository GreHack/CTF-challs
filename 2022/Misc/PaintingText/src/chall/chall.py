from flask import Flask, request, jsonify
import subprocess
import re
app = Flask(__name__)

@app.route('/', methods = ['GET', 'POST'])
def index():
    print("/ reached !")
    if request.method == 'POST':
        print("POST")
        parameter = request.json.get("parameter", "true")
        print(parameter)
        output = list()

        blacklist = ['less', 'more', 'nl', 'tail', 'head', 'strings', 'flag', ';', '&', '|', '<', '>', '$', '(', ')']

        try:
            if any(forbidden in parameter for forbidden in blacklist):
                sh = '/usr/games/fortune'
            else:
                safe_param = re.sub(r"(\s+)", "\u00A0", parameter.replace("'", "\\'"))
                sh = f"figlet -f big $'{safe_param}'"
            print(sh)
            command = subprocess.check_output(sh, shell=True, executable='/bin/bash').decode('utf8')
            output.append("```")
            output.append(f"{command}")
            output.append("```")
        except Exception as e:
            output.append("Something failed. RIP!")
        return jsonify({"output": "\n".join(output)})
    else:
        return 'Right path, bad method!'

@app.route('/health', methods = ['GET'])
def health():
    return 'alive!'


if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=8000)
