#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "";
const char* password = "";

WebServer server(80);

const int greenPin1 = 21;
const int yellowPin1 = 19;
const int redPin1 = 18;

const int greenPin2 = 15;
const int yellowPin2 = 16;
const int redPin2 = 17;

bool light1Active = false;
bool light2Active = false;
bool bothActive = false;

void runSynchronizedSequence() {
  //green
  digitalWrite(greenPin1, HIGH); digitalWrite(redPin2, HIGH); 
  delay(3000); 
  digitalWrite(greenPin1, LOW); 
  delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); digitalWrite(greenPin1, LOW); delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); digitalWrite(greenPin1, LOW); delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); 
  digitalWrite(greenPin1, LOW);  
  delay(100); 
  // yellow
  digitalWrite(yellowPin1, HIGH);
  delay(2500); 
  digitalWrite(yellowPin1, LOW); digitalWrite(redPin2, LOW); 
  delay(75);
  // idk
  digitalWrite(greenPin2, HIGH); digitalWrite(redPin1, HIGH); 
  delay(3000); 
  digitalWrite(greenPin2, LOW); 
  delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); digitalWrite(greenPin2, LOW); delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); digitalWrite(greenPin2, LOW); delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); 
  digitalWrite(greenPin2, LOW);  
  delay(100); 
  // yellow
  digitalWrite(yellowPin2, HIGH);
  delay(2500); 
  digitalWrite(yellowPin2, LOW); digitalWrite(redPin1, LOW); 
  delay(75);
}

void runLight1Sequence() {
  digitalWrite(yellowPin1, LOW);
  // green
  digitalWrite(greenPin1, HIGH);
  delay(3000); 
  digitalWrite(greenPin1, LOW); 
  delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); digitalWrite(greenPin1, LOW); delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); digitalWrite(greenPin1, LOW); delay(100);
  digitalWrite(greenPin1, HIGH); delay(500); 
  digitalWrite(greenPin1, LOW);  
  delay(100); 
  
  // yellow
  digitalWrite(yellowPin1, HIGH);
  delay(2500); 
  digitalWrite(yellowPin1, LOW);
  delay(75);
  
  // red
  digitalWrite(redPin1, HIGH);
  delay(3000);
  digitalWrite(redPin1, LOW);
  delay(75);
}

void runLight2Sequence() {
  digitalWrite(yellowPin2, LOW);
  // green
  digitalWrite(greenPin2, HIGH);
  delay(3000); 
  digitalWrite(greenPin2, LOW); 
  delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); digitalWrite(greenPin2, LOW); delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); digitalWrite(greenPin2, LOW); delay(100);
  digitalWrite(greenPin2, HIGH); delay(500); 
  digitalWrite(greenPin2, LOW);  
  delay(100); 
  
  // yellow
  digitalWrite(yellowPin2, HIGH);
  delay(2500); 
  digitalWrite(yellowPin2, LOW);
  delay(75);
  
  // red
  digitalWrite(redPin2, HIGH);
  delay(3000);
  digitalWrite(redPin2, LOW);
  delay(75);
}

void setLightYellow(int lightNumber) {
  if (lightNumber == 1) {
    digitalWrite(greenPin1, LOW);
    digitalWrite(yellowPin1, HIGH);
    digitalWrite(redPin1, LOW);
  } else {
    digitalWrite(greenPin2, LOW);
    digitalWrite(yellowPin2, HIGH);
    digitalWrite(redPin2, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(greenPin1, OUTPUT);
  pinMode(yellowPin1, OUTPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  pinMode(redPin2, OUTPUT);
  
  setLightYellow(1);
  setLightYellow(2);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/status/light1-on", HTTP_GET, []() {
    Serial.println("Status: Light 1 is ON - Starting sequence");
    bothActive = false;
    light1Active = true;
    light2Active = false;
    server.send(200, "text/plain", "Light 1 is ON");
  });

  server.on("/status/light2-on", HTTP_GET, []() {
    Serial.println("Status: Light 2 is ON - Starting sequence");
    bothActive = false;
    light1Active = false;
    light2Active = true;
    server.send(200, "text/plain", "Light 2 is ON");
  });

  server.on("/status/both-on", HTTP_GET, []() {
    Serial.println("Status: Both lights are ON - Starting synchronized sequence");
    bothActive = true;
    light1Active = false;
    light2Active = false;
    server.send(200, "text/plain", "Both lights are ON");
  });

  server.on("/status/all-off", HTTP_GET, []() {
    Serial.println("Status: All lights are OFF");
    bothActive = false;
    light1Active = false;
    light2Active = false;
    setLightYellow(1);
    setLightYellow(2);
    server.send(200, "text/plain", "All lights are OFF");
  });

  server.on("/", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });
  
  server.begin();
}

void loop() {
  server.handleClient();
  
  if (bothActive) {
    runSynchronizedSequence();
  } else if (light1Active) {
    runLight1Sequence();
  } else if (light2Active) {
    runLight2Sequence();
  }
}