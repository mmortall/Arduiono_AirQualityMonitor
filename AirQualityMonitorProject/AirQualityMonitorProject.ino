/// Sketch for Air Quality monitor device
// Uses ard_adagfx_ili9341_xpt2046_Display
// CO2 sensor
// EEROM for storing values 

#define USE_SENSORS
#define USE_PRESURE_SENSOR

#include <SoftwareSerial.h>
#include "ard_adagfx_ili9341_xpt2046_Display.h"

#ifdef USE_SENSORS
#include "SensorsManager.h"
SensorsManager _Sensors;
#endif  

ard_adagfx_ili9341_xpt2046_Display _Display = ard_adagfx_ili9341_xpt2046_Display();

#include "MeasurementsScreen.h"
MeasurementsScreen _MeasurementsScreen = MeasurementsScreen();


enum EMenu
{
  EMenu_Measurements = 0,
  EMenu_Chart = 1,
  EMenu_Options = 2
} CurrMenu;

void setup() {
  Serial.begin(9600);

#ifdef USE_SENSORS
  _Sensors.Init();
#endif  

  _Display.init();
  _MeasurementsScreen.init();
}

int counter = 0;

bool dustSensorUpdate;

void loop() 
{
#ifdef USE_SENSORS
  if(dustSensorUpdate)
  {
    dustSensorUpdate = !_Sensors.DustSensorUpdate();
    return;
  }
  else
  {
      _Sensors.Update();
      dustSensorUpdate = true;
  }
#endif

///_Sensors.Update();

  char buff[16];
  //memset(&buff[0], 0, sizeof(buff));
  //itoa(_Sensors.GetCO2(), buff, 10);
  //strcat(buff, " ppm");
  //char buff1[16] = {0};
  //sprintf(buff1, "%d ppm", _Sensors.GetCO2());
  _MeasurementsScreen.CO2Val->setLabel(String(_Sensors.GetCO2()) + " ppm");

  //memset(&buff[0], 0, sizeof(buff));
  dtostrf(_Sensors.GetTemp(), 4, 1, buff);
  //strcat(buff, " C");
  //char buff2[16] = {0};
  //sprintf(buff2, "%.2d ppm", _Sensors.GetTemp());
  _MeasurementsScreen.TemperVal->setLabel(String(buff) + " C");

  //memset(&buff[0], 0, sizeof(buff));
  dtostrf(_Sensors.GetPresure(), 4, 1, buff);
  //strcat(buff, " мрс");
  //char buff3[16] = {0};
  //sprintf(buff3, "%.2d мрс", _Sensors.GetPresure());
  _MeasurementsScreen.PresVal->setLabel(String(buff) + " мрс");

  memset(buff, 0, sizeof(buff));
  short dustVal = _Sensors.GetDust();
  if(dustVal > 0)
  {
    itoa(_Sensors.GetDust(), buff, 10);
    strcat(buff, " ug");
    strcat(buff, "/m3");
    _MeasurementsScreen.DustVal->setLabel(buff);
    _MeasurementsScreen.DustVal->setTextColor(clrGREEN);
  }
  else
  {
    _MeasurementsScreen.DustVal->setLabel("измеряю");    
    _MeasurementsScreen.DustVal->setTextColor(clrGRAY);
  }


  _Display.update();
  _MeasurementsScreen.update(_Display);

  delay(300L);
  counter++;
}
