#include<WiFi.h>
#include<ArduinoMqttClient.h>
#include<DHT.h>

const char *ssid = "POCO";
const char *password = "12348765";

const char *host = "172.22.53.21";
unsigned int port = 1883;

#define DHT_PIN 4
#define DHT_TYPE  DHT11
#define MQ2_PIN 34   
int gasValue = 0;

DHT dht(DHT_PIN, DHT_TYPE);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature();
  Serial.print("temp=");
  Serial.println(temp);

  WiFiClient wifiClient;
  MqttClient publisher(wifiClient);

  if(publisher.connect(host, port)){
    publisher.beginMessage("temp");
    publisher.print(temp);
    publisher.endMessage();


    
  }

  float hum = dht.readHumidity();

  Serial.print("humidity=");
  Serial.println(hum);

  if(publisher.connect(host, port)){
    publisher.beginMessage("humidity");
    publisher.print(hum);
    publisher.endMessage();


  }

  gasValue = analogRead(MQ2_PIN); 

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > 300) 
  {   
    Serial.println("Smoke Detected!");
  } 
  else 
  {
    Serial.println("Smoke not present");
  }
    if(publisher.connect(host, port)){
    publisher.beginMessage("gas");
    publisher.print(gasValue);
    publisher.endMessage();


  }

  delay(10000); 
}











