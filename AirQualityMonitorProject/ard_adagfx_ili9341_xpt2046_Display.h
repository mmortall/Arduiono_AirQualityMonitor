#ifndef ard_adagfx_ili9341_xpt2046_Display_h
#define ard_adagfx_ili9341_xpt2046_Display_h

#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>				// must include this here (or else IDE can't find it) 

#include <Adafruit_GFX.h>				// PDQ: Core graphics library
#include "PDQ_ILI9341_config.h"			// PDQ: ST7735 pins and other setup for this sketch
#include <Adafruit_ILI9341.h>			// PDQ: Hardware-specific driver library
#include <U8g2_for_Adafruit_GFX.h>
//#include <Fonts/FreeMonoBoldOblique12pt7b.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

#define ttclk    9  
#define ttcs    6    
#define ttdin   7     
#define ttdout  8     
#define ttirq  12 

#include <URTouch.h>
URTouch  myTouch = URTouch(ttclk, ttcs, ttdin, ttdout, ttirq); //URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);

#include "IDisplay.h"


#define clrBLACK ILI9341_BLACK
#define clrWHITE ILI9341_WHITE

class ard_adagfx_ili9341_xpt2046_Display : public IDisplay
{
  bool m_IsTouch;
  short m_TouchX;
  short m_TouchY;

public:
	void init()
	{
		delay(200);

		tft.begin();
		u8g2_for_adafruit_gfx.begin(tft);
		tft.setRotation(1);

		myTouch.InitTouch();
		myTouch.setPrecision(PREC_MEDIUM);

		u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
		u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
		u8g2_for_adafruit_gfx.setFont(u8g2_font_10x20_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
	}

	void update()
	{
		if (myTouch.dataAvailable() == true)
		{
			m_IsTouch = true;

			myTouch.read();
			m_TouchX = myTouch.getX();
			m_TouchY = myTouch.getY();
		}
		else
		{
			m_IsTouch = false;
		}
	}

	void setCursor(short x, short y)
	{
		tft.setCursor(x, y);
	}

	void setTextColor(short color)
	{
    u8g2_for_adafruit_gfx.setForegroundColor(color);      // apply Adafruit GFX color
	}

	void print(const char* txt)
	{
		u8g2_for_adafruit_gfx.print(txt);
	}

	bool isTouch()
	{
		return m_IsTouch;
	}

	short getTouchX()
	{
		return m_TouchX;
	}

	short getTouchY()
	{
		return m_TouchY;
	}
};


#endif
