/// Sketch for Air Quality monitor device
// Uses ard_adagfx_ili9341_xpt2046_Display
// CO2 sensor
// EEROM for storing values 

#define USE_SENSORS

#include <SoftwareSerial.h>
#include "ard_adagfx_ili9341_xpt2046_Display.h"

#ifdef USE_SENSORS
#include "SensorsManager.h"
SensorsManager _Sensors = SensorsManager();
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

void loop() 
{
#ifdef USE_SENSORS
  _Sensors.Update();
#endif

  /*char buff[100];
  memset(buff, 0, 100);
  strcat(buff, "Test: ");
  strcat(buff, String(counter).c_str());
  Serial.println(buff);
  _MeasurementsScreen.CO2Btn->setLabel(buff);*/

  _Display.update();
  _MeasurementsScreen.update(_Display);

  delay(300L);
  counter++;
}
