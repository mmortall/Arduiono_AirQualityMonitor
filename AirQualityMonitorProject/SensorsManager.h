#ifndef SENSORS_MANAGER_H
#define SENSORS_MANAGER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <AM2320.h>
#include <Adafruit_MPL3115A2.h> //presure sensor

#undef  MPL3115A2_ADDRESS
#define MPL3115A2_ADDRESS                       (0x61)

AM2320 th;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX

#define gasPinAnalog 7

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

class SensorsManager
{
public:
  SensorsManager():m_CO2ppm(-1), m_Presure(-1), m_Temperature(-1000), m_Humidity(-1), m_GAS(-1), m_Dust(-1), m_T1(0)
  {
  }

	void Init() 
	{
    mySerial.begin(9600);
	};
  
	void Update() 
	{
    //read data from CO2 sensor
    mySerial.write(cmd, 9);
    memset(response, 0, 9);
    mySerial.readBytes(response, 9);
  
    int i;
    byte crc = 0;
    for (i = 1; i < 8; i++) crc+=response[i];
    crc = 255 - crc;
    crc++;
  
    //display.setTextColor(WHITE);
    //display.setCursor(15,5);
    //DisplayPrint("CO2: "); DisplayPrintln("n/a");
  
    //gas sensor
    float gasSensorValue = max(0, (((float)analogRead(gasPinAnalog)) - 100.0f) / (800.0f - 100.0f) * 100.0f);
    m_GAS = gasSensorValue;
    
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
      Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
  
      //DisplayPrint("CO2: "); DisplayPrint("n/a ("+ String(crc) +")");
      m_CO2ppm = -1;
    } else {
      unsigned int responseHigh = (unsigned int) response[2];
      unsigned int responseLow = (unsigned int) response[3];
      unsigned int ppm = (256*responseHigh) + responseLow;
  
      double res = 0;
  
      res = (double)ppm - (double)1000;
      //updateBrightness(ledPin, res / 300.0, ppm);
  
      res = (double)ppm - (double)2000;
      //updateBrightness(ledPinAlarm, res / 500.0, ppm);
  
      res = (double)ppm - (double)3000;
      //updateBrightness(ledPinPesdecPolniy, res / 500.0, ppm);
  
      //Serial.print(ppm);
  
      //display.setTextColor(WHITE);
      //DisplaySetCursor(15,5);
      //DisplayPrint("CO2: "); DisplayPrint((int)ppm); DisplayPrint(" ppm");
      m_CO2ppm = ppm;
    }
  
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
  
        m_T1 = th.t;
        m_Humidity = th.h;
  
        //DisplaySetCursor(15,25);
        //DisplayPrint("Hm: "); DisplayPrint(th.h);DisplayPrint(" %");
        break;
    }
  
    //presure sensor
    if (! baro.begin()) {
      Serial.println("Couldnt find presure sensor");
    }
    else
    {   
      float pascals = baro.getPressure();
      Serial.print((int)pascals*0.00750062); Serial.println(" mmrs");
      //DisplaySetCursor(15,35);
      //DisplayPrint("P: "); DisplayPrint((float)(pascals*0.00750062)); DisplayPrint(" mmrs");
      m_Presure = (float)(pascals*0.00750062);
    
      float altm = baro.getAltitude();
      m_Altitude = altm;
      Serial.print(altm); Serial.println(" meters");
      //DisplaySetCursor(15,45);
      //DisplayPrint("Hsea: "); DisplayPrint(altm); DisplayPrint(" m");
  
      float tempC = baro.getTemperature();
      if(m_T1 == 0)
        m_T1 = tempC;
        
      tempC = (tempC + m_T1) / 2.0f;
      m_Temperature = tempC;
      //DisplaySetCursor(15,15);
      //DisplayPrint("T: "); DisplayPrint(tempC); DisplayPrint(" "); DisplayPrint((char)247); DisplayPrint("C"); //temparature
    }
	};

	short GetCO2();

private:
	short m_CO2ppm;
	float m_Presure;
	float m_Temperature;
  short m_Humidity;
  float m_Altitude; //in meters
	short m_GAS;
	short m_Dust;

  float m_T1; 
};

#endif
