#define USE_ADAFRUT_DIPLAY
//#define USE_TOUCH_DIPLAY

#include <SoftwareSerial.h>
#include <Wire.h>
#include <AM2320.h>
#include <Adafruit_MPL3115A2.h> //presure sensor

#ifdef USE_ADAFRUT_DIPLAY
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);
#endif

#ifdef USE_TOUCH_DIPLAY
#include <SPI.h>
#include <TFT.h>
#endif

#undef  MPL3115A2_ADDRESS
#define MPL3115A2_ADDRESS                       (0x61)

#ifdef USE_TOUCH_DIPLAY
#define SCLK (4)
#define MOSI (5)
#define CS   (6)
#define DC   (7)
#define RESET (8) 
TFT TFTscreen = TFT(CS, DC, MOSI);
#endif

AM2320 th;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX

float T1 = 0; 

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int led = 10;      
int ledPin = 9;       
int ledPinAlarm = 8;  
int ledPinPesdec = 7;  
int ledPinPesdecPolniy = 6;
int gasPinAnalog = 7;
int brightness = 0;    // how bright the LED is
int fadeAmount = 20;    // how many points to fade the LED by

#ifdef PLAY_SOUND
int soundPin = 11;
#endif

unsigned short _CurrDiplayCursorX;
unsigned short _CurrDiplayCursorY;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(ledPinAlarm, OUTPUT);
  pinMode(ledPinPesdec, OUTPUT);
  pinMode(ledPinPesdecPolniy, OUTPUT);
  pinMode(led, OUTPUT);

#ifdef PLAY_SOUND
  pinMode(soundPin, OUTPUT);
#endif

#ifdef USE_ADAFRUT_DIPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else
  TFTscreen.begin();
  TFTscreen.background(0,0,0);
  TFTscreen.stroke(255,255,255); //test color
  TFTscreen.setTextSize(2);
#endif

  DisplayRender();
}

void loop() 
{
  ClearDisplay();
  
  mySerial.write(cmd, 9);
  memset(response, 0, 9);
  mySerial.readBytes(response, 9);

  /*if (!mySerial.available()){
    delay(1000);
    return;
  }*/

  //mySerial.read();
  //if (mySerial.available() >= 8) {
  //}
  /*else 
  {
    return;
  }*/

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

    res = (double)ppm - (double)700;
    updateBrightness(ledPin, res / 300.0, ppm);

    res = (double)ppm - (double)1000;
    updateBrightness(ledPinAlarm, res / 500.0, ppm);

    res = (double)ppm - (double)1500;
    updateBrightness(ledPinPesdec, res / 500.0, ppm);    

    res = (double)ppm - (double)2000;
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

void ClearDisplay()
{
#ifdef USE_ADAFRUT_DIPLAY  
  display.clearDisplay();
#else

#endif  
}

void DisplaySetCursor(int x, int y)
{
  _CurrDiplayCursorX = x;
  _CurrDiplayCursorY = y;
#ifdef USE_ADAFRUT_DIPLAY  
  display.setCursor(x,y);
#else

#endif    
}

void DisplayPrint(String val)
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.print(val);
#else
TFTscreen.text(val, _CurrDiplayCursorX, _CurrDiplayCursorY);
#endif    
}


void DisplayPrint(char val)
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.print(val);
#else
TFTscreen.text(val, _CurrDiplayCursorX, _CurrDiplayCursorY);
#endif    
}

void DisplayPrint(int val)
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.print(val);
#else
TFTscreen.text(val, _CurrDiplayCursorX, _CurrDiplayCursorY);
#endif    
}

void DisplayPrint(float val)
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.print(val);
#else
  TFTscreen.text(val, _CurrDiplayCursorX, _CurrDiplayCursorY);
#endif    
}
void DisplayRender()
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.display();
#endif    
}
  

#ifdef PLAY_SOUND
void playSoundSimple()
{
  tone(soundPin, 440, 250);
delay(250);
tone(soundPin, 165, 250);
delay(250);
tone(soundPin, 440, 250);
delay(250);
delay(250);

tone(soundPin, 440, 250);
delay(250);
tone(soundPin, 165, 250);
delay(250);
tone(soundPin, 440, 250);
delay(250);
delay(250);

tone(soundPin, 330, 125);
delay(125);
tone(soundPin, 294, 125);
delay(125);
tone(soundPin, 262, 125);
delay(124);
tone(soundPin, 494, 125);
delay(125);
tone(soundPin, 440, 125);
delay(125);
tone(soundPin, 494, 125);
delay(125);
tone(soundPin, 262, 125);
delay(125);
tone(soundPin, 294, 125);
delay(125);

tone(soundPin, 330, 250);
delay(250);
tone(soundPin, 165, 250);
delay(250);
tone(soundPin, 440, 250);
}

void playSoundMario()
{
  tone(soundPin,1318,150);
delay(150);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1568,600);
delay(600);
tone(soundPin,784,600);
delay(600); 
tone(soundPin,1046,450) ; // 2й такт
delay(450);
tone(soundPin,784,150);
delay(450);
tone(soundPin,659,300);
delay(450);
tone(soundPin,880,300);
delay(300);
tone(soundPin,987,150);
delay(300);
tone(soundPin,932,150);
delay(150);
tone(soundPin,880,300);
delay(300);
tone(soundPin,784,210); // 3й такт
delay(210);
tone(soundPin,1318,210);
delay(210);
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1750,300);
delay(300);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1568,150);
delay(300);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
tone(soundPin,987,450);
delay(450);
tone(soundPin,1046,450); // 4й такт
delay(450);
tone(soundPin,784,150);
delay(450);
tone(soundPin,659,300);
delay(450);
tone(soundPin,880,300);
delay(300);
tone(soundPin,987,150);
delay(300);
tone(soundPin,932,150);
delay(150);
tone(soundPin,880,300);
delay(300);
tone(soundPin,784,210); // 5й такт
delay(210);
tone(soundPin,1318,210);
delay(210);
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1750,300);
delay(300);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1568,150);
delay(300);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
tone(soundPin,987,450);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
delay(300); // 7ой 
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,2093,300);
delay(300);
tone(soundPin,2093,150);
delay(150);
tone(soundPin,2093,600);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз ПОВТОР
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(450);
tone(soundPin,1244,300);
delay(450);
tone(soundPin,1174,450);
delay(450);
tone(soundPin,1046,600);
delay(1200 );
tone(soundPin,1568,150); // 6ой Спуск вниз
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
delay(300); // 7ой 
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,2093,300);
delay(300);
tone(soundPin,2093,150);
delay(150);
tone(soundPin,2093,600);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз ПОВТОР
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(450);
tone(soundPin,1244,300);
delay(450);
tone(soundPin,1174,450);
delay(450);
tone(soundPin,1046,600);
delay(1200);
tone(soundPin,1046,150); // 8ой такт
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,784,600);
delay(600);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
delay(1200);
tone(soundPin,1046,150); // 8ой такт
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,784,600);
delay(600);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
delay(1200);
tone(soundPin,1046,450) ; // 2й такт
delay(450);
tone(soundPin,784,150);
delay(450);
tone(soundPin,659,300);
delay(450);
tone(soundPin,880,300);
delay(300);
tone(soundPin,987,150);
delay(300);
tone(soundPin,932,150);
delay(150);
tone(soundPin,880,300);
delay(300);
tone(soundPin,784,210); // 3й такт
delay(210);
tone(soundPin,1318,210);
delay(210);
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1750,300);
delay(300);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1568,150);
delay(300);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
tone(soundPin,987,450);
delay(450);
tone(soundPin,1046,450); // 4й такт
delay(450);
tone(soundPin,784,150);
delay(450);
tone(soundPin,659,300);
delay(450);
tone(soundPin,880,300);
delay(300);
tone(soundPin,987,150);
delay(300);
tone(soundPin,932,150);
delay(150);
tone(soundPin,880,300);
delay(300);
tone(soundPin,784,210); // 5й такт
delay(210);
tone(soundPin,1318,210);
delay(210);
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1750,300);
delay(300);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1568,150);
delay(300);
tone(soundPin,1318,300);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
tone(soundPin,987,450);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
delay(300); // 7ой 
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,2093,300);
delay(300);
tone(soundPin,2093,150);
delay(150);
tone(soundPin,2093,600);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз ПОВТОР
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(450);
tone(soundPin,1244,300);
delay(450);
tone(soundPin,1174,450);
delay(450);
tone(soundPin,1046,600);
delay(1200 );
tone(soundPin,1568,150); // 6ой Спуск вниз
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(150);
delay(300); // 7ой 
tone(soundPin,1568,150);
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,2093,300);
delay(300);
tone(soundPin,2093,150);
delay(150);
tone(soundPin,2093,600);
delay(600);
tone(soundPin,1568,150); // 6ой Спуск вниз ПОВТОР
delay(150);
tone(soundPin,1480,150);
delay(150);
tone(soundPin,1396,150);
delay(150);
tone(soundPin,1244,300);
delay(300);
tone(soundPin,1318,150);
delay(300);
tone(soundPin,830,150);
delay(150);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,150);
delay(450);
tone(soundPin,1244,300);
delay(450);
tone(soundPin,1174,450);
delay(450);
tone(soundPin,1046,600);
delay(1200);
tone(soundPin,1046,150); // 8ой такт
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,784,600);
delay(600);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
delay(1200);
tone(soundPin,1046,150); // 8ой такт
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1318,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,880,150);
delay(150);
tone(soundPin,784,600);
delay(600);
tone(soundPin,1046,150);
delay(150);
tone(soundPin,1046,300);
delay(300);
tone(soundPin,1046,150);
delay(300);
tone(soundPin,784,150);
delay(150);
tone(soundPin,1174,300);
delay(300);
tone(soundPin,1046,2400);
}

void playSound() 
{
  int shortTone = 250;
  int longTone = 500;
  tone(soundPin, 700, 300);
  delay(longTone);
  tone(soundPin, 700, 300);
  delay(longTone);
  tone(soundPin, 780, 150);
  delay(shortTone);
  tone(soundPin, 700, 150);
  delay(shortTone);
  tone(soundPin, 625, 450);
  delay(longTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 460, 450);
  delay(longTone);
  tone(soundPin, 350, 450);
  delay(longTone);
  delay(longTone);
  tone(soundPin, 350, 450);
  delay(longTone);
  tone(soundPin, 460, 450);
  delay(longTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 625, 450);
  delay(longTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 700, 1350);
  delay(longTone * 3);
  tone(soundPin, 700, 300);
  delay(longTone);
  tone(soundPin, 700, 300);
  delay(longTone);
  tone(soundPin, 780, 150);
  delay(shortTone);
  tone(soundPin, 700, 150);
  delay(shortTone);
  tone(soundPin, 625, 450);
  delay(longTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 460, 450);
  delay(longTone);
  tone(soundPin, 350, 450);
  delay(longTone);
  delay(longTone);
  tone(soundPin, 350, 450);
  delay(longTone);
  tone(soundPin, 625, 450);
  delay(longTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 700, 450);
  delay(longTone);
  tone(soundPin, 590, 150);
  delay(shortTone);
  tone(soundPin, 520, 150);
  delay(shortTone);
  tone(soundPin, 460, 1350);
}

#endif
