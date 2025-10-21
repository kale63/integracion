#include <WiFi.h>
#include <WebServer.h> 

const char* ssid = "";
const char* password = "";

const int pinPot = 33;
const int freq = 5000;
float volt = 0.0;
int pot = 0;

WebServer server(80);

void handlePWMValue() {
  pot = analogRead(pinPot);
  volt = (pot * 3.3) / 4095;
  Serial.print("POT value: ");
  Serial.print(pot);
  Serial.print(" | Voltage: ");
  Serial.println(volt);
  
  ledcWrite(21, pot);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  
  String response = String(volt, 2);
  server.send(200, "text/plain", response);
}

void handleNotFound() {
  String message = "File Not Found\n\nURI: ";
  message += server.uri();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  ledcAttach(21, freq, 12);

  Serial.print("Conectao a");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectao.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/pwm/value", HTTP_GET, handlePWMValue);
  server.onNotFound(handleNotFound);
  
  server.begin(); 
  Serial.println("HTTP iniciado.");
}

void loop() {
  server.handleClient();
}