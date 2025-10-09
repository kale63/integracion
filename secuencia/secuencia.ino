#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "";
const char* password = "";

const int led1 = 21;
const int led2 = 19;
const int led3 = 18;

int dir = 0;
bool sequenceRunning = false;

WebServer server(80);

void runSequence() {
  if (dir == 0) {
    // Forward sequence
    digitalWrite(led1, HIGH); delay(250); digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH); delay(250); digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH); delay(250); digitalWrite(led3, LOW);
  } else {
    // Backward sequence
    digitalWrite(led3, HIGH); delay(250); digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH); delay(250); digitalWrite(led2, LOW);
    digitalWrite(led1, HIGH); delay(250); digitalWrite(led1, LOW);
  }
}

void handleSequence() {
  Serial.println("Sequence trigger received from web - Inverting direction");
  dir = !dir;
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  String response = (dir == 0) ? "Dirección cambiada a: ADELANTE" : "Dirección cambiada a: ATRÁS";
  server.send(200, "text/plain", response);
  
  Serial.println(response);
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

void handleNotFound() {
  String message = "File Not Found\n\nURI: ";
  message += server.uri();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/sequence", HTTP_GET, handleSequence);
  server.on("/sequence", HTTP_OPTIONS, handleOptions);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
  
  runSequence();
}
