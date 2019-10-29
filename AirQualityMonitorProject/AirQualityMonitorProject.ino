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
    dustSensorUpdate = _Sensors.DustSensorUpdate();
    return;
  }
  else
  {
      _Sensors.Update();
      dustSensorUpdate = true;
  }
#endif

  char buff[16];
  itoa(_Sensors.GetCO2(), buff, 10);
  strcat(buff, " ppm");
  _MeasurementsScreen.CO2Val->setLabel(buff);

  dtostrf(_Sensors.GetTemp(), 4, 1, buff);
  strcat(buff, " C");
  _MeasurementsScreen.TemperVal->setLabel(buff);

  dtostrf(_Sensors.GetPresure(), 4, 1, buff);
  strcat(buff, " мрс");
  _MeasurementsScreen.PresVal->setLabel(buff);

  short dustVal = _Sensors.GetDust();
  itoa(_Sensors.GetDust(), buff, 10);
  //strcat(buff, " ugm");
  _MeasurementsScreen.DustVal->setLabel(buff);
  _MeasurementsScreen.DustVal->setTextColor(clrGREEN);
  /*if(dustVal > 0)
  {
    itoa(_Sensors.GetDust(), buff, 10);
    strcat(buff, " ug/m3");
    _MeasurementsScreen.DustVal->setLabel(buff);
    _MeasurementsScreen.DustVal->setTextColor(clrGREEN);
  }
  else
  {
    itoa(_Sensors.GetDustSensorNextResultSec(), buff, 10);
    strcat(buff, " с.");
    _MeasurementsScreen.DustVal->setLabel(buff);    
    _MeasurementsScreen.DustVal->setTextColor(clrGRAY);
  }*/


  _Display.update();
  _MeasurementsScreen.update(_Display);

  delay(300L);
  counter++;
}
