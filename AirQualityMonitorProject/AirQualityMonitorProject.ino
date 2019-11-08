/// Sketch for Air Quality monitor device
// Uses ard_adagfx_ili9341_xpt2046_Display
// CO2 sensor
// EEROM for storing values 

#define USE_SENSORS
//#define USE_PRESURE_SENSOR
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

enum ECharts
{
  ECharts_None = 0,
  
  ECharts_CO2 = 1,
  ECharts_Quality = 2,
  ECharts_Temp = 3,
  ECharts_Humidity = 4,  

  ECharts_Last,
  ECharts_First = ECharts_CO2,
  
} CurrChart;

void setup() {
  Serial.begin(9600);
  eep_setup();
  

#ifdef USE_SENSORS
  _Sensors.Init();
#endif  

  _Display.init();
  _MeasurementsScreen.init();

  CurrChart = ECharts_First;
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
  bool isCO2ready = _Sensors.GetCO2() > 500;
  if(isCO2ready)
  {
    _MeasurementsScreen.CO2Val->setLabel("Концентр. CO2: " + String(_Sensors.GetCO2()) + String(" ppm"));
    _MeasurementsScreen.CO2Val->setTextColor(GetCO2Color(_Sensors.GetCO2()));
  }
  else
  {
     _MeasurementsScreen.CO2Val->setLabel("Концентр. CO2: " + String("измеряю"));
     _MeasurementsScreen.CO2Val->setTextColor(clrWHITE);
  }

  dtostrf(_Sensors.GetTemp(), 4, 1, buff);
  strcat(buff, " C");
  _MeasurementsScreen.TemperVal->setLabel("Температура: " + String(buff));
  _MeasurementsScreen.TemperVal->setTextColor(GetTempQualityColor(_Sensors.GetTemp()*10));

  /*dtostrf(_Sensors.GetPresure(), 4, 1, buff);
  strcat(buff, " мрс");
  _MeasurementsScreen.PresVal->setLabel("Атмос. давление: " + String(buff));
  _MeasurementsScreen.PresVal->setTextColor(clrWHITE);*/
 
  _MeasurementsScreen.HumidityVal->setLabel("Влажность: "+ String(_Sensors.GetHumidity()) + String(" %"));
  _MeasurementsScreen.HumidityVal->setTextColor(GetHumidityColor(_Sensors.GetHumidity()));
 
  _MeasurementsScreen.QualityVal->setLabel("Загрязнен. воздуха: " + String(_Sensors.GetGAS()) + String(" %"));
  _MeasurementsScreen.QualityVal->setTextColor(GetAirQualityColor(_Sensors.GetGAS()));

  if(isCO2ready)
    addDataBlock(_Sensors.GetCO2(), (uint16_t)floor(_Sensors.GetTemp()*10), _Sensors.GetHumidity(), _Sensors.GetGAS());

  //drawing chart
  uint16_t numOfBlocks = eep_read_header_num_of_blocks();  
  uint16_t numOfData = numOfBlocks;
  uint16_t numOfDataEnd = ((numOfBlocks <= 320) ? 0 : (numOfBlocks - 320 - 1));
  uint16_t i = numOfData-1;

  uint16_t chartH = 140;
  uint16_t min_val = ~0;
  uint16_t max_val = 0;
  while(true)
  {
    uint16_t co2; uint16_t temp; uint16_t hum; uint16_t gass;
    if(getDataBlock(i, co2, temp, hum, gass) < 0)
      break;

    uint16_t val_for_char = co2;
    switch(CurrChart)
    {
      case ECharts_CO2: val_for_char = co2; break;
      case ECharts_Quality: val_for_char = gass; break;
      case ECharts_Temp: val_for_char = temp; break;
      case ECharts_Humidity: val_for_char = hum; break; 
    };
    
    if(val_for_char < min_val)
      min_val = val_for_char;
    if(val_for_char > max_val)
      max_val = val_for_char;
            
    if(i == numOfDataEnd)
      break;

    i--;  
  }

  _Display.fillRect(0, (int)(240 - chartH), 320, 240, clrBLACK);
  i = numOfData-1;
  while(true)
  {
    uint16_t co2; uint16_t temp; uint16_t hum; uint16_t gass;
    if(getDataBlock(i, co2, temp, hum, gass) < 0)
      break;

    //draw chart 
    uint16_t val_for_char = co2;
    short bar_color = clrWHITE;
    switch(CurrChart)
    {
      case ECharts_CO2: val_for_char = co2; bar_color = GetCO2Color(val_for_char); break;
      case ECharts_Quality: val_for_char = gass; bar_color = GetAirQualityColor(val_for_char); break;
      case ECharts_Temp: val_for_char = temp; bar_color = GetTempQualityColor(val_for_char); break;
      case ECharts_Humidity: val_for_char = hum; bar_color = GetHumidityColor(val_for_char); break; 
    };
    
    uint16_t val_on_char = (uint16_t)(chartH * ((float)(val_for_char - min_val) / (float)(max_val - min_val)));
    _Display.drawVLine((i - numOfDataEnd), 240 - val_on_char, 240 - val_on_char, bar_color);
    
    if(i == numOfDataEnd)
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
  CurrChart = (ECharts)((int)CurrChart + 1);
  if(CurrChart == ECharts_Last)
    CurrChart = ECharts_First;
}
