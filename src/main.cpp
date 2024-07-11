#include <ConfigurableFirmata.h>
#include <WiFi.h>
#include "Wire.h"

#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <FirmataExt.h>
FirmataExt firmataExt;

void sysexCallback(byte, byte, byte*);
void systemResetCallback();

void initTransport()
{
  Firmata.begin(115200);
}

void initFirmata()
{
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  firmataExt.addFeature(analogInput); // Adiciona a funcionalidade de entrada analógica ao Firmata
  
  Firmata.attach(SYSTEM_RESET, systemResetCallback);
  Firmata.attach(START_SYSEX, sysexCallback);
}

void setup()
{
  initFirmata();
  initTransport();
  
  pinMode(2, OUTPUT);
}

void loop()
{
  while (Firmata.available()) {
    Firmata.processInput();
  }
}

void systemResetCallback()
{
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      Firmata.setPinMode(i, ANALOG);
    } else if (IS_PIN_DIGITAL(i)) {
      Firmata.setPinMode(i, OUTPUT);
    }
  }
  firmataExt.reset();
}

void sysexCallback(byte command, byte argc, byte *argv)
{
  if (command == 0x01) {  // Exemplo de comando SYSEX para controlar o LED
    byte led_state = argv[0];  // Primeiro argumento determina o estado do LED

    if (led_state == 0x01) {
      digitalWrite(2, HIGH);  
    } else if (led_state == 0x00) {
      digitalWrite(2, LOW);   
    }
  }
  firmataExt.handleSysex(command, argc, argv);
}
