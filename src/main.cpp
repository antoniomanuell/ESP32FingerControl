#include <ConfigurableFirmata.h>
#include <WiFi.h>
#include <Wire.h>

const int ledPin = 2;

void sysexCallback(byte command, byte argc, byte *argv);
void systemResetCallback();

void setup() {
  Serial.begin(115200);  
  Firmata.begin(115200);
  Firmata.attach(SYSTEM_RESET, systemResetCallback);
  Firmata.attach(START_SYSEX, sysexCallback);

  pinMode(ledPin, OUTPUT);
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
}

void sysexCallback(byte command, byte argc, byte *argv) {
  Serial.print("Recebido comando SYSEX: ");  // Mensagem de depuração
  Serial.println(command, HEX);
  
  if (command == 0x01) {  // Comando SYSEX para controlar o LED
    byte led_state = argv[0];  // Primeiro argumento determina o estado do LED
    Serial.print("Estado do LED: ");
    Serial.println(led_state);

    if (led_state == 0x01) {
      digitalWrite(ledPin, HIGH);  
      Serial.println("LED aceso"); 
    } else if (led_state == 0x00) {
      digitalWrite(ledPin, LOW);  
      Serial.println("LED apagado");
    }
  }
}
