#include <WiFi.h>
#include <WebServer.h> 

const char* ssid = "";
const char* password = "";

const int pin21 = 21;
const int pin19 = 19;
float volt = 0.0;
int sliderValue = 0;

WebServer server(80);


void handlePotValue() {
  // Get slider value from query parameter
  if (server.hasArg("value")) {
    sliderValue = server.arg("value").toInt();
  }
  
  volt = (sliderValue * 3.3) / 4095;
  
  Serial.print("Slider value: ");
  Serial.print(sliderValue);
  Serial.print(" | Voltage: ");
  Serial.println(volt);
  
  // Control LEDs based on voltage
  if (volt >= 1.5 && volt < 2.2) {
    digitalWrite(pin21, HIGH);
    digitalWrite(pin19, LOW);
  } else if (volt >= 2.2) {
    digitalWrite(pin21, LOW);
    digitalWrite(pin19, HIGH);
  } else {
    digitalWrite(pin21, LOW);
    digitalWrite(pin19, LOW);
  }
  
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
  pinMode(pin21, OUTPUT);
  pinMode(pin19, OUTPUT);
  digitalWrite(pin21, LOW);
  digitalWrite(pin19, LOW);

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

  server.on("/pot/value", HTTP_GET, handlePotValue);
  server.onNotFound(handleNotFound);
  
  server.begin(); 
  Serial.println("HTTP iniciado.");
}

void loop() {
  server.handleClient();
}