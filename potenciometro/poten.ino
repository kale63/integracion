#include <WiFi.h>
#include <WebServer.h> 

const char* ssid = "";
const char* password = "";

const int ledPin = 18;

WebServer server(80); 

void handleOn() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON command received.");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200, "text/plain", "LED is ON");
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF command received.");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200, "text/plain", "LED is OFF");
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
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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

  server.on("/led/on", HTTP_GET, handleOn);
  server.on("/led/off", HTTP_GET, handleOff);
  server.onNotFound(handleNotFound);
  
  server.begin(); 
  Serial.println("HTTP iniciado.");
}

void loop() {
  server.handleClient();
}