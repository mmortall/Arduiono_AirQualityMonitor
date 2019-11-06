/// Sketch for Air Quality monitor device
// Uses ard_adagfx_ili9341_xpt2046_Display
// CO2 sensor
// EEROM for storing values 

#define USE_SENSORS
#define USE_PRESURE_SENSOR
//#define USE_DUST_SENSOR

#include <SoftwareSerial.h>
#include "ard_adagfx_ili9341_xpt2046_Display.h"

#include "EEROMUtls.h"

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
  eep_setup();
  

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
#ifdef USE_DUST_SENSOR
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
#else
    _Sensors.Update();
#endif
#endif

#ifdef USE_SENSORS
  char buff[16];
  //memset(&buff[0], 0, sizeof(buff));
  itoa(_Sensors.GetCO2(), buff, 10);
  strcat(buff, " ppm");
  _MeasurementsScreen.CO2Val->setLabel(buff);

  dtostrf(_Sensors.GetTemp(), 4, 1, buff);
  strcat(buff, " C");
  _MeasurementsScreen.TemperVal->setLabel(buff);

  dtostrf(_Sensors.GetPresure(), 4, 1, buff);
  strcat(buff, " мрс");
  _MeasurementsScreen.PresVal->setLabel(buff);

  itoa(_Sensors.GetHumidity(), buff, 10);
  strcat(buff, " %");
  _MeasurementsScreen.HumidityVal->setLabel(buff);

  itoa(_Sensors.GetGAS(), buff, 10);
  strcat(buff, " %");
  _MeasurementsScreen.QualityVal->setLabel(buff);

  addDataBlock(_Sensors.GetCO2(), _Sensors.GetTemp(), _Sensors.GetHumidity(), _Sensors.GetGAS());

  //drawing chart
  uint16_t numOfBlocks = eep_read_header_num_of_blocks();  
  uint16_t numOfData = min(320, numOfBlocks);
  uint16_t i = numOfData-1;

  uint16_t chartH = 110;
  uint16_t min_co2_val = 5000;
  uint16_t max_co2_val = 0;
  while(true)
  {
    uint16_t co2; uint16_t temp; uint16_t hum; uint16_t pres;
    if(getDataBlock(i, co2, temp, hum, pres) < 0)
      break;

    if(co2 < min_co2_val)
      min_co2_val = co2;
    if(co2 > max_co2_val)
      max_co2_val = co2;
            
    if(i == 0)
      break;

    i--;  
  }

  _Display.fillRect(0, (int)(240 - chartH), 320, 240, clrBLACK);
  i = numOfData-1;
  while(true)
  {
    uint16_t co2; uint16_t temp; uint16_t hum; uint16_t pres;
    if(getDataBlock(i, co2, temp, hum, pres) < 0)
      break;

    //draw chart 
    uint16_t co2_on_char = (uint16_t)(chartH * ((float)(co2 - min_co2_val) / (float)(max_co2_val - min_co2_val)));
    _Display.drawLine(i, 240 - co2_on_char, i, 240, clrRED); 

    if(i == 0)
      break;

    i--;  
  }
#endif

#ifdef USE_DUST_SENSOR
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
#endif

  _Display.update();
  _MeasurementsScreen.update(_Display);

  delay(300L);
  counter++;
}
