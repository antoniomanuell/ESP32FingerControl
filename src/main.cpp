#include <ConfigurableFirmata.h>
#include <WiFi.h>
#include <Wire.h>

// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor B
int motor2Pin1 = 17;
int motor2Pin2 = 16;
int enable2Pin = 4;

const char* ssid = "UNITEL_5G";
const char* password = "alma@2012";

WiFiServer server(80);

void moveForward();
void stopMotors();
void turnLeft();
void turnRight();
void moveBackward();
void handleClient(WiFiClient client);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  // Motor A pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // Motor B pins
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Enable motors
  digitalWrite(enable1Pin, HIGH);
  digitalWrite(enable2Pin, HIGH);

  // Stop motors
  stopMotors();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
  }
}

void handleClient(WiFiClient client) {
  while (client.connected()) {
    if (client.available()) {
      String command = client.readStringUntil('\n');
      if (command == "MOVE_FORWARD") {
        moveForward();
      } else if (command == "STOP") {
        stopMotors();
      } else if (command == "TURN_LEFT") {
        turnLeft();
      } else if (command == "TURN_RIGHT") {
        turnRight();
      } else if (command == "MOVE_BACKWARD") {
        moveBackward();
      }
      client.println("OK");
    }
  }
  client.stop();
}

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void turnLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void turnRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}