#include <Arduino.h>

#include <ArduinoBearSSL.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <ArduinoECCX08.h>
#include <Arduino_JSON.h>

#include "arduino_secrets.h"

// Enter your sensitive data in arduino_secrets.h
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

const char broker[] = SECRET_BROKER;

const char* deviceCertificate  = SECRET_CERTIFICATE;
const char* privateCertificate  = AWS_CERT_PRIVATE;

const char* topic = "topic";

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

unsigned long lastMillis = 0;
unsigned long publishFrequency = 20000; // publish every 20 seconds


void setup() {
  Serial.println("setup");
  if (Serial.available()) {
    Serial.begin(115200);
    while (!Serial);
  }

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setKey(privateCertificate, deviceCertificate);

}

void loop() {
  JSONVar jsonObject;

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  
  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  lastMillis += 1;

  if (lastMillis > publishFrequency) {
    lastMillis = 0;

    mqttClient.beginMessage(topic);

    jsonObject["message"] = "Hello World";
    String jsonString = JSON.stringify(jsonObject);

    mqttClient.print(jsonString);
    mqttClient.endMessage();

    Serial.println("published message");
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print(String("Attempting to connect to SSID: ") + ssid);
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed try again
    Serial.print(".");
    delay(3000);
  }
  Serial.println("");

  Serial.println(String("Now connected to: ") + ssid);
}

void connectMQTT() {
  Serial.println(String("Attempting to connect to MQTT endpoint: ") + broker);
  
  while (!mqttClient.connect(broker, 8883)) {
    // failed try again
    delay(3000);
    Serial.println(String("MQTT Connection Error: ") + mqttClient.connectError());
  }
  Serial.println(String("Now connected to: ") + broker);
}
