#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "NUMERICABLE-f0b_Ext";
const char *password = "1a2b3c4d5e";

const char *host = "http://192.168.0.34:3306/api/sensors";
const char *hostPUT = "http://192.168.0.34:3306/api/sensors/JHGFe-dsjfhg-ze-fdhg";

int sensorPin = A0;

boolean freshStart = false;

void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.println("Connecting to the network");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("ssid");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int rawvoltage = analogRead(sensorPin);
  float millivolts = (rawvoltage/1024.0)*3300;
  float celcius = millivolts/10;
  Serial.println(celcius);

  if(freshStart) addSensorToDatabase();
  else{
    HTTPClient http;
    WiFiClient wifiClient;

    String putData = "{\"value\":\"" + String(celcius) + "\"}";

    http.begin(wifiClient, hostPUT);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PUT(putData);

    String payload = http.getString();

    Serial.println(httpCode);
  
    http.end();
  }
  delay(1000);
}

void addSensorToDatabase(){
  HTTPClient http;
  WiFiClient wifiClient;
  
  String postData = "{\"name\":\"Temperature Sensor 1\", \"macAddress\":\"JHGFe-dsjfhg-ze-fdhg\"}";

  http.begin(wifiClient, host);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(postData);

  String payload = http.getString();

  Serial.println(httpCode);

  http.end();
  freshStart = false;
  delay(1000);
}
