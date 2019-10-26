//example read temperatur and humidity information and save it to flash memory every 1 sec
//after 10 sec it reads data and print to Serial

#include <SoftwareSerial.h>
#include <Wire.h>
#include <AM2320.h>
#include <EEPROM.h>
#include <extEEPROM.h>

#define AM2320_address (0xB8 >> 1) 
#define EEPROM_24LC512_address 0x50

AM2320 th;
extEEPROM myEEPROM(kbits_256, 1, 64); //one 24LC256 EEPROMS on the bus

void setup() {
  Serial.begin(9600);
}

void loop() 
{
  int ths = th.Read();
  switch(ths) {
    case 2:
      Serial.println("CRC failed");
      break;
    case 1:
      Serial.println("Sensor offline");
      break;
    case 0:
      Serial.print("humidity: ");
      Serial.print(th.h);
      Serial.print("%, temperature: ");
      Serial.print(th.t);
      Serial.println("*C");
      break;
  }

  delay(5000);
}


