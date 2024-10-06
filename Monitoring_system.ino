#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Monitoring_system.h"
#include "Adafruit_VEML6070.h"
#include "DHT.h"


WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_VEML6070 uv = Adafruit_VEML6070(); // create VEML6070 object
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  uv.begin(VEML6070_1_T);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(4000);

 int Tempreture = dht.readTemperature();
 int Humedity = dht.readHumidity();
 int Gas_co = MQ2_sensor_read();
  int UV = uv.readUV();

  Serial.print("\nTempreture: ");
  Serial.print(Tempreture);
  if (client.publish(Martuis_DHT_Temperature, String(Tempreture).c_str(), true)) {
    Serial.println("Published successfully");
  } else {
    Serial.println("Publish failed");
  }

  Serial.print(" \nHumedity: ");
  Serial.print(Humedity);
  if (client.publish(Martuis_DHT_Humedity , String(Humedity).c_str(), true)) {
    Serial.println(" - Published successfully");
  } else {
    Serial.println(" - Publish failed");
  }

  Serial.print("\n Co: ");
  Serial.print(Gas_co);
  if (client.publish(Martuis_CO2_Gas, String(Gas_co).c_str(), true)) {
    Serial.println(" - Published successfully");
  } else {
    Serial.println(" - Publish failed");
  }

  Serial.print("\n UV level: ");
  Serial.print(UV);
  if (client.publish(Martuis_VEML6070_UV   , String(UV).c_str(), true)) {
    Serial.println(" - Published successfully");
  } else {
    Serial.println(" - Publish failed");
  }
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("\nAttempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
