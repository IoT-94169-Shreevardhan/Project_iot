from flask import Flask, render_template, request, jsonify

server = Flask(__name__)

sensor_data = {
    "temp": "--",
    "humidity": "--",
    "gas": "--",
}

@server.route("/")
def dashboard():
    return render_template("dashboard.html")

@server.route("/update", methods=["POST"])
def update_data():
    data = request.get_json()  

    if data is None:
        return "Invalid JSON", 400

    sensor_data["temp"] = data.get("temp")
    sensor_data["humidity"] = data.get("humidity")
    sensor_data["gas"] = data.get("gas")

    print(sensor_data)
    return jsonify({"status": "success"}), 200

@server.route("/data")
def get_data():
    return jsonify(sensor_data)

if __name__ == "__main__":
    server.run(host="0.0.0.0", port=5000, debug=True)