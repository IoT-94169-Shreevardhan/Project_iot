#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <HTTPClient.h>
#include <DHT.h>

const char *ssid = "D stay 105";
const char *password = "dstay105";

const char *host = "192.168.1.157";
unsigned int port = 1883;

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define MQ2_PIN 34

int gasValue = 0;

DHT dht(DHT_PIN, DHT_TYPE);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void connectToBroker() {
  Serial.print("Connecting to MQTT broker... ");
  while (!mqttClient.connect(host, port)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n✅ Broker connected successfully!");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  connectToBroker();   // 🔹 Broker connection message here
}

void loop() {

  if (!mqttClient.connected()) {
    connectToBroker();
  }

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  gasValue = analogRead(MQ2_PIN);

  Serial.print("Temp: ");
  Serial.println(temp);

  Serial.print("Humidity: ");
  Serial.println(hum);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > 1000) {
    Serial.println("🚨 Smoke Detected!");
  } else {
    Serial.println("✅ Smoke not present");
  }

  // Publish temperature
  mqttClient.beginMessage("temp");
  mqttClient.print(temp);
  mqttClient.endMessage();

  // Publish humidity
  mqttClient.beginMessage("humidity");
  mqttClient.print(hum);
  mqttClient.endMessage();

  // Publish gas value
  mqttClient.beginMessage("gas");
  mqttClient.print(gasValue);
  mqttClient.endMessage();

  // HTTP POST
  String body = "{\"temp\":" + String(temp) +
                ",\"humidity\":" + String(hum) +
                ",\"gas\":" + String(gasValue) + "}";

  HTTPClient httpClient;
  httpClient.begin(wifiClient, "http://172.22.53.21:5000/update");
  httpClient.addHeader("Content-Type", "application/json");

  int status = httpClient.POST(body);
  Serial.printf("HTTP status = %d\n", status);
  httpClient.end();

  delay(10000);
}