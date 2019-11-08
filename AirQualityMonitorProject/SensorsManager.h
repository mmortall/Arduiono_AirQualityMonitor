#ifndef SENSORS_MANAGER_H
#define SENSORS_MANAGER_H

#include <Arduino.h>
#if DEBUG
#include <SoftwareSerial.h>
#endif

#include <Wire.h>
#include <AM2320.h>

#ifdef USE_PRESURE_SENSOR
#include <Adafruit_MPL3115A2.h> //presure sensor
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
#endif

#undef  MPL3115A2_ADDRESS
#define MPL3115A2_ADDRESS                       (0x61)

AM2320 th;

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX

#define gasPinAnalog 7

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

#define MaxStorageHistory 10000

#ifdef USE_DUST_SENSOR
//dust sensor
/////////////////// for DSM501 sensor /////////////////////////////////////////

#define pinV1 (A2)
#define pinV2 (A3)
unsigned long durationV1;
unsigned long durationV2;
unsigned long starttimeV1;
unsigned long starttimeV2;
unsigned long lowpulseoccupancyV1 = 0;
unsigned long lowpulseoccupancyV2 = 0;
float one_to_two_point_five;
float ratioV1 = 0;
float ratioV2 = 0;
boolean flagV1 = true;
boolean flagV2 = true;
boolean V1 = false;
boolean V2 = false;
float concentrationV1 = 0;
float concentrationV2 = 0;
int allV1, allV2, goodV1, goodV2;

//#define MinDustMeasurementTimeSec (1 * 60) //5 min

unsigned long starttime;
unsigned long endtime;
#define sampletime_ms (30000L)
#endif

short invalidValueCounter = 0;

short GetCO2Color(uint16_t val)
{
  return val < 900 ? clrGREEN : (val < 1600 ? clrYELLOW : clrRED);
}

short GetAirQualityColor(uint16_t val)
{
  return val < 30 ? clrGREEN : (val < 60 ? clrYELLOW : clrRED);
}

short GetHumidityColor(uint16_t val)
{
  return (val >= 350 && val <= 650) ? clrGREEN : ((val >= 200 && val <= 800) ? clrYELLOW : clrRED);
}

short GetTempQualityColor(uint16_t val)
{
  return (val <= 270 && val >= 200) ? clrGREEN : (val > 270 ? clrRED : clrBLUE);
}

////////////////////////////////////////////////////////////////////////////////////
class SensorsManager
{
public:
  SensorsManager():m_CO2ppm(-1), m_Presure(-1), m_Temperature(-1000), m_Humidity(-1), m_GAS(-1), m_Dust(-1), m_T1(0), m_DustCalcCounter(0), m_DustOverall(0)
  {
    m_CO2ppmArr = new short[MaxStorageHistory];
  }

	void Init() 
	{
    mySerial.begin(9600);

#ifdef USE_DUST_SENSOR
    pinMode(pinV1,INPUT);
    pinMode(pinV2,INPUT);
    starttime = millis(); 
#endif
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

    //gas sensor
    float gasSensorValue = max(0, (((float)analogRead(gasPinAnalog)) - 250.0f) / (500.0f - 250.0f) * 100.0f);
    m_GAS = gasSensorValue;
    
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
#ifdef DEBUG      
      Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
#endif      

      m_CO2ppm = -1;
    } else {
      unsigned int responseHigh = (unsigned int) response[2];
      unsigned int responseLow = (unsigned int) response[3];
      unsigned int ppm = (256*responseHigh) + responseLow;

      m_CO2ppm = ppm;
    }

    int ths = th.Read();
    switch(ths) {
#ifdef DEBUG        
      case 2:      
        Serial.println("CRC failed");
        break;
      case 1:
        Serial.println("Sensor offline");
        break;
#endif         
      case 0:
#ifdef DEBUG        
        Serial.print("humidity: ");
        Serial.print(th.h);
        Serial.print("%, temperature: ");
        Serial.print(th.t);
        Serial.println("*C");
#endif     
        short newHum = floor(th.h * 10);
        if(m_Humidity > 0 && m_Temperature > -900 && abs(newHum - m_Humidity) > 60 && abs(th.t - m_Temperature) > 20 && invalidValueCounter < 10 )
        {
          invalidValueCounter++;
#ifdef DEBUG        
          Serial.println("Sensor return invalid value: ");
#endif          
          break;
        }
        invalidValueCounter = 0;
        m_T1 = th.t;
        m_Humidity = floor(th.h * 10);  
        m_Temperature = m_T1; 
        break;
    }

#ifdef USE_PRESURE_SENSOR  
    //presure sensor + temperature
    if (! baro.begin()) {
#ifdef DEBUG_PRESURE_SENSOR
      Serial.println("Couldnt find presure sensor");
#endif
    }
    else
    {   
      float pascals = baro.getPressure();
#ifdef DEBUG_PRESURE_SENSOR      
      Serial.print((int)pascals*0.00750062); Serial.println(" mmrs");
#endif      
      m_Presure = (float)(pascals*0.00750062);
    
      float altm = baro.getAltitude();
      m_Altitude = altm;
#ifdef DEBUG_PRESURE_SENSOR      
      Serial.print(altm); Serial.println(" meters");
#endif         

      float tempC = baro.getTemperature();
      if(m_T1 == 0)
        m_T1 = tempC;
        
      m_Temperature = (tempC + m_T1) / 2.0f;
    }
#else
  m_Temperature = m_T1;    
#endif    
	};

  bool DustSensorUpdate()
  {
#ifdef USE_DUST_SENSOR    
      ////////////  DSM 501 ////////////////////////
  
        V1 = digitalRead(pinV1);
    if ((flagV1)&&!(V1)) {         // start period of low V1
        starttimeV1 = millis();
        flagV1 = false;
    };
    if (!(flagV1)&&(V1)) {         // stop period of low V1
        durationV1 = millis() - starttimeV1;
        flagV1 = true;
        allV1+=1;
   
      if ((durationV1 <= 90)&&(durationV1 >= 10)) {
        lowpulseoccupancyV1 += durationV1;
        goodV1+=1;
      };
    };
  
    V2 = digitalRead(pinV2);
    if ((flagV2)&&!(V2)) {         // start period of low V2
        starttimeV2 = millis();
        flagV2 = false;
    };
    if (!(flagV2)&&(V2)) {         // stop period of low V2
        durationV2 = millis() - starttimeV2;
        flagV2 = true;
        allV2+=1;
   
    if ((durationV2 <= 90)&&(durationV2 >= 10)) {
      lowpulseoccupancyV2 += durationV2;
      goodV2+=1;
    };
   };

    endtime = millis();
    if ((endtime-starttime) > sampletime_ms)
    {
      ratioV1 = (lowpulseoccupancyV1*100.0)/(endtime-starttime);  // Integer percentage 0=>100
      ratioV2 = (lowpulseoccupancyV2*100.0)/(endtime-starttime);  // Integer percentage 0=>100
        //Serial.print("ratioV1:"); Serial.println(ratioV1);
         //Serial.print("ratioV2:"); Serial.println(ratioV2);
          concentrationV1 = 1.1*pow(ratioV1,3)-3.8*pow(ratioV1,2)+520*ratioV1+0.62; // using spec sheet curve in pcs in 1/100 ft3
          concentrationV2 = 1.1*pow(ratioV2,3)-3.8*pow(ratioV2,2)+520*ratioV2+0.62; // using spec sheet curve in pcs in 1/100 ft3
      
        //concentrationV1 = -0.0885*pow(ratioV1,4) - 2.55055*pow(ratioV1,3)- 21.920538*pow(ratioV1,2) + 172.171285*ratioV1 - 90.112;
        //concentrationV2 = -0.0885*pow(ratioV2,4) - 2.55055*pow(ratioV2,3)- 21.920538*pow(ratioV2,2) + 172.171285*ratioV2 - 90.112;
        //Serial.print("concentrationV1:"); Serial.println(concentrationV1);
        // Serial.print("concentrationV2:"); Serial.println(concentrationV2);
               
        if (concentrationV1 < 0) {concentrationV1 = 0.0;};
        if (concentrationV2 < 0) {concentrationV2 = 0.0;};
        one_to_two_point_five = concentrationV2 - concentrationV1;
        if (one_to_two_point_five < 0) {one_to_two_point_five = 0;};

        float one_to_two_point_five_AQI = one_to_two_point_five * 3534.0 * 1.8e-5;
      
        //Serial.print("  DSM501_>2.5: ");
        //Serial.print(concentrationV1);
      
        //Serial.print("  DSM501_>1.0: ");
        //Serial.print(concentrationV2);
          
        //Serial.print("  DSM501_1-2.5: ");
        //Serial.print(one_to_two_point_five);
        //Serial.println();
        //m_DustOverall += (int) one_to_two_point_five_AQI;
       
        lowpulseoccupancyV1 = 0;
        lowpulseoccupancyV2 = 0;
        loops = 0;
        allV1 = 0;
        goodV1 = 0;
        allV2 = 0;
        goodV2 = 0;
        m_DustCalcCounter++;

        if(one_to_two_point_five_AQI > 0)
          m_Dust = (short)floor(one_to_two_point_five_AQI);

        //Serial.print("one_to_two_point_five_AQI:"); Serial.println(one_to_two_point_five_AQI);  
        starttime = millis(); 
        return true;
    }   // if 30 sec passed*/

    //m_DustSensorDataUpdateLeft = MinDustMeasurementTimeSec - (((m_DustCalcCounter * sampletime_ms)/1000) + msecDiff/1000);
    return false;

    #else
    return true;
    #endif

  }

	short GetCO2() { return m_CO2ppm; }
  float GetPresure() { return m_Presure; }
  float GetTemp() { return m_Temperature; }
  short GetAltitude() { return m_Altitude; }
  short GetHumidity() { return m_Humidity; }
  short GetGAS() { return m_GAS; }
  short GetDust() { return m_Dust; }
  short GetDustSensorNextResultSec() { return m_DustSensorDataUpdateLeft; }
  
private:  
	short m_CO2ppm;
	float m_Presure;
	float m_Temperature;
  short m_Humidity; //real hum * 10
  short m_Altitude; //in meters
	short m_GAS;
	short m_Dust;

  short m_DustOverall;
  short m_DustCalcCounter;
  short m_DustSensorDataUpdateLeft;

  short* m_CO2ppmArr;

  float m_T1; 
};

#endif
