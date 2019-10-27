/// Sketch for Air Quality monitor device
// Uses ard_adagfx_ili9341_xpt2046_Display
// CO2 sensor
// EEROM for storing values 

#include <SoftwareSerial.h>
#include "ard_adagfx_ili9341_xpt2046_Display.h"

#include "SensorsManager.h"
#include "MeasurementsScreen.h"

SensorsManager _Sensors = SensorsManager();
ard_adagfx_ili9341_xpt2046_Display _Display = ard_adagfx_ili9341_xpt2046_Display();

enum EMenu
{
  EMenu_Measurements = 0,
  EMenu_Chart = 1,
  EMenu_Options = 2
} CurrMenu;

void setup() {
  Serial.begin(9600);

  _Sensors.Init();
  _Display.init();
}

void loop() 
{
  _Sensors.Update();
  _Display.update();
}
