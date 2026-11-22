from flask import Flask, request, jsonify

app = Flask(__name__)

data = {}

@app.route("/")
def index():
    return """
<h1>Water Monitor</h1>
<p>Server is running</p>
"""

@app.route("/api/water", methods=["POST"])
def recieve_water_data():
    global data

    current_data = request.get_json()

    if not(current_data):
        return jsonify({"error": "no data recieved"}), 400

    data = current_data
    print("recieved data: ", data)

    return jsonify({"status": "ok"}), 200


if __name__=="__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)


