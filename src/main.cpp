#include <WiFi.h>
#include <WiFiUdp.h>
#include "Wire.h"

// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor B
int motor2Pin1 = 17;
int motor2Pin2 = 16;
int enable2Pin = 4;

const char* ssid = "Nome_da_rede";
const char* password = "palavra_passe";

WiFiUDP udp;
unsigned int localUdpPort = 4210;  // Porta UDP
char incomingPacket[255];  // Buffer para dados recebidos

void moveForward();
void stopMotors();
void turnLeft();
void turnRight();
void moveBackward();
void handleClient(const char* command);

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

  udp.begin(localUdpPort);

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
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.printf("Recebido pacote UDP de tamanho %d\n", packetSize);
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
      Serial.printf("Pacote recebido: %s\n", incomingPacket);
      handleClient(incomingPacket);
    }
  }
}

void handleClient(const char* command) {
  Serial.printf("Comando recebido: %s\n", command);
  if (strcmp(command, "MOVE_FORWARD") == 0) {
    Serial.println("Movendo para frente");
    moveForward();
  } else if (strcmp(command, "STOP") == 0) {
    Serial.println("Parando motores");
    stopMotors();
  } else if (strcmp(command, "TURN_LEFT") == 0) {
    Serial.println("Virando à esquerda");
    turnLeft();
  } else if (strcmp(command, "TURN_RIGHT") == 0) {
    Serial.println("Virando à direita");
    turnRight();
  } else if (strcmp(command, "MOVE_BACKWARD") == 0) {
    Serial.println("Movendo para trás");
    moveBackward();
  } else {
    Serial.println("Comando desconhecido");
  }
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
