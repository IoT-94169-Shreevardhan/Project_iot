import requests
import time
from executeSelectquery import executeSelectQuery



THINGSPEAK_API_KEY = "8IJM9FG1CQERML6F"
THINGSPEAK_URL = "https://api.thingspeak.com/update"

while True:

    query=f"""SELECT Temp, Humidity, Gas_Level
        FROM Sensor_Data
        ORDER BY timestamp DESC
        LIMIT 1"""
    row =executeSelectQuery(query=query)

    if row is not None:
        temp = row[0]
        humidity = row[1]
        gas = row[2]

        payload = {
            "api_key": THINGSPEAK_API_KEY,
            "field1": temp,
            "field2": humidity,
            "field3": gas
        }

        response = requests.get(THINGSPEAK_URL, params=payload)

        if response.status_code == 200:
            print("✅ Data uploaded to ThingSpeak")
        else:
            print("❌ Upload failed, status:", response.status_code)

    else:
        print("⏳ No data in database")

    # ThingSpeak minimum update interval
    time.sleep(15)