import paho.mqtt.client as mqtt
from datetime import datetime 
from executequery import executeQuery

sensor_data = {
    "temp": None,
    "humidity": None,
    "gas": None
}

def on_message(client, userdata, message):
    topic = message.topic
    value = message.payload.decode()

    print(f"Received → {topic} : {value}")

    if topic in sensor_data:
        sensor_data[topic] = value


    if all(sensor_data.values()):
        print("✅ All sensor values received")
        print(sensor_data)

        temp=sensor_data["temp"]
        humidity=sensor_data["humidity"]
        gas=sensor_data["gas"]

        query=f"insert into Sensor_Data values('{datetime.now()}',{temp},{humidity},{gas});"
        executeQuery(query=query)

        for key in sensor_data:
            sensor_data[key] = None


# creat a client to  subscribe topic
subscriber = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# add on_message into our subscriber
subscriber.on_message = on_message

# send connect message to publisher
subscriber.connect("localhost")

# subscribe for topic
subscriber.subscribe("temp")
subscriber.subscribe("humidity")
subscriber.subscribe("gas")

print("📡 Waiting for temp, humidity and gas data...")

# keep subscriber running continuously
subscriber.loop_forever()