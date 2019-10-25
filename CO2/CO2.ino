//#define PLAY_SOUND
//#define USE_ADAFRUT_DIPLAY
#define USE_TOUCH_DIPLAY

#include <SoftwareSerial.h>
#include <Wire.h>
#include <AM2320.h>
#include <Adafruit_MPL3115A2.h> //presure sensor

#include "displays_settings.h"

#undef  MPL3115A2_ADDRESS
#define MPL3115A2_ADDRESS                       (0x61)

AM2320 th;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX

float T1 = 0; 

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int led = 10;      
int ledPin = 9;       
int ledPinAlarm = 8;  
int ledPinPesdecPolniy = 7;  
int gasPinAnalog = 7;
int brightness = 0;    // how bright the LED is
int fadeAmount = 20;    // how many points to fade the LED by

#ifdef PLAY_SOUND
int soundPin = 11;
#include "sounds_functions.h"
#endif

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(ledPinAlarm, OUTPUT);
  pinMode(ledPinPesdecPolniy, OUTPUT);
  pinMode(led, OUTPUT);

#ifdef PLAY_SOUND
  pinMode(soundPin, OUTPUT);
#endif

#ifdef USE_ADAFRUT_DIPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else
  TFTscreen.begin();
  TFTscreen.fillScreen(ILI9341_BLACK);
  //TFTscreen.stroke(255,255,255); //test color
#endif

  DisplayRender();
}

void loop() 
{
  ClearDisplay();
  SetTextColor(ILI9341_WHITE);

  TFTscreen.setTextSize(5);
  DisplaySetCursor(0, 0);
  DisplayPrint("Test");
  delay(500);

  return;

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
  DisplaySetCursor(15,55);
  DisplayPrint("Gas: "); DisplayPrint(gasSensorValue); DisplayPrint(" %");
  
  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
    Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));

    DisplaySetCursor(15,5);
    DisplayPrint("CO2: "); DisplayPrint("n/a ("+ String(crc) +")");
    
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    unsigned int ppm = (256*responseHigh) + responseLow;

    double res = 0;

    res = (double)ppm - (double)1000;
    updateBrightness(ledPin, res / 300.0, ppm);

    res = (double)ppm - (double)2000;
    updateBrightness(ledPinAlarm, res / 500.0, ppm);

    res = (double)ppm - (double)3000;
    updateBrightness(ledPinPesdecPolniy, res / 500.0, ppm);

    //Serial.print(ppm);

    //display.setTextColor(WHITE);
    DisplaySetCursor(15,5);
    DisplayPrint("CO2: "); DisplayPrint((int)ppm); DisplayPrint(" ppm");
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

      T1 = th.t;

      DisplaySetCursor(15,25);
      DisplayPrint("Hm: "); DisplayPrint(th.h);DisplayPrint(" %");
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
    DisplaySetCursor(15,35);
    DisplayPrint("P: "); DisplayPrint((float)(pascals*0.00750062)); DisplayPrint(" mmrs");
  
    float altm = baro.getAltitude();
    Serial.print(altm); Serial.println(" meters");
    DisplaySetCursor(15,45);
    DisplayPrint("Hsea: "); DisplayPrint(altm); DisplayPrint(" m");

    float tempC = baro.getTemperature();
    if(T1 == 0)
      T1 = tempC;
      
    tempC = (tempC + T1) / 2.0f;
    DisplaySetCursor(15,15);
    DisplayPrint("T: "); DisplayPrint(tempC); DisplayPrint(" "); DisplayPrint((char)247); DisplayPrint("C"); //temparature
  }

  DisplayRender();
  
  delay(5000);

  //playSoundSimple();

  //updateBrightnessTest();
}

void updateBrightness(int pin, double level, int ppm)
{
  level = min(1.0, level);
  level = max(0.0, level);

  int lvl = (int)(255.0 * level);
  analogWrite(pin, lvl);

  Serial.print(pin);
  Serial.print(": ");
  Serial.print(lvl); 
  Serial.print(": ");
  Serial.println(ppm);
   
  //delay(30);
}

// the loop routine runs over and over again forever:
void updateBrightnessTest() {

  analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  //delay(1000);
}

