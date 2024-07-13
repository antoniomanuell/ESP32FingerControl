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

void moveForward();
void stopMotors();
void turnLeft();
void turnRight();
void moveBackward();
void sysexCallback(byte command, byte argc, byte *argv);
void systemResetCallback();

void setup() {
  Serial.begin(115200);  
  Firmata.begin(115200);
  Firmata.attach(SYSTEM_RESET, systemResetCallback);
  Firmata.attach(START_SYSEX, sysexCallback);

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
  while (true) {
    if (Firmata.available()) {
      Serial.println("Firmata disponível, processando entrada...");
      Firmata.processInput();
    } else {
      Serial.println("Firmata não disponível, esperando...");
    }
    delay(1000);
  }
}

void systemResetCallback() {
  // Callback de reset do sistema
}

void sysexCallback(byte command, byte argc, byte *argv) {
  if (command == 0x01) {
    byte motor_command = argv[0];
    switch (motor_command) {
      case 0x01: // Move forward
        moveForward();
        break;
      case 0x00: // Stop motors
        stopMotors();
        break;
      case 0x02: // Turn left
        turnLeft();
        break;
      case 0x03: // Turn right
        turnRight();
        break;
      case 0x04: // Move backward
        moveBackward();
        break;
      default:
        stopMotors();
        break;
    }
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
