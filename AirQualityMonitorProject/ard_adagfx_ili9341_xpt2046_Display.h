#ifndef ard_adagfx_ili9341_xpt2046_Display_h
#define ard_adagfx_ili9341_xpt2046_Display_h

//#define USE_U8g2

//#include <SoftwareSerial.h>
//#include <Wire.h>
#include <SPI.h>				// must include this here (or else IDE can't find it) 

#include <Adafruit_GFX.h>				// PDQ: Core graphics library
#include "PDQ_ILI9341_config.h"			// PDQ: ST7735 pins and other setup for this sketch

static const unsigned char font[] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x55, 0x51, 0x55, 0x3E, 
0x3E, 0x6B, 0x6F, 0x6B, 0x3E, 
0x0C, 0x1E, 0x3C, 0x1E, 0x0C, 
0x08, 0x1C, 0x3E, 0x1C, 0x08, 
0x1C, 0x4A, 0x7F, 0x4A, 0x1C, 
0x18, 0x5C, 0x7F, 0x5C, 0x18, 
0x00, 0x1C, 0x1C, 0x1C, 0x00, 
0x7F, 0x63, 0x63, 0x63, 0x7F, 
0x00, 0x1C, 0x14, 0x1C, 0x00, 
0x7F, 0x63, 0x6B, 0x63, 0x7F, 
0x30, 0x48, 0x4D, 0x33, 0x07, 
0x06, 0x29, 0x79, 0x29, 0x06, 
0x20, 0x50, 0x3F, 0x02, 0x0C, 
0x60, 0x7F, 0x05, 0x35, 0x3F, 
0x2A, 0x1C, 0x77, 0x1C, 0x2A, 
0x00, 0x7F, 0x3E, 0x1C, 0x08, 
0x08, 0x1C, 0x3E, 0x7F, 0x00, 
0x14, 0x22, 0x7F, 0x22, 0x14, 
0x00, 0x5F, 0x00, 0x5F, 0x00, 
0x06, 0x09, 0x7F, 0x01, 0x7F, 
0x4A, 0x55, 0x55, 0x55, 0x29, 
0x60, 0x60, 0x60, 0x60, 0x60, 
0x54, 0x62, 0x7F, 0x62, 0x54, 
0x08, 0x04, 0x7E, 0x04, 0x08, 
0x08, 0x10, 0x3F, 0x10, 0x08, 
0x08, 0x08, 0x2A, 0x1C, 0x08, 
0x08, 0x1C, 0x2A, 0x08, 0x08, 
0x1C, 0x10, 0x10, 0x10, 0x10, 
0x1C, 0x3E, 0x08, 0x3E, 0x1C, 
0x30, 0x3C, 0x3F, 0x3C, 0x30, 
0x06, 0x1E, 0x7E, 0x1E, 0x06, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x5F, 0x00, 0x00, 
0x00, 0x07, 0x00, 0x07, 0x00, 
0x14, 0x7F, 0x14, 0x7F, 0x14, 
0x24, 0x2A, 0x7F, 0x2A, 0x12, 
0x23, 0x13, 0x08, 0x64, 0x62, 
0x36, 0x49, 0x56, 0x20, 0x50, 
0x00, 0x00, 0x07, 0x00, 0x00, 
0x00, 0x1C, 0x22, 0x41, 0x00, 
0x00, 0x41, 0x22, 0x1C, 0x00, 
0x14, 0x08, 0x3E, 0x08, 0x14, 
0x08, 0x08, 0x3E, 0x08, 0x08, 
0x00, 0xA0, 0x60, 0x00, 0x00, 
0x08, 0x08, 0x08, 0x08, 0x08, 
0x00, 0x60, 0x60, 0x00, 0x00, 
0x20, 0x10, 0x08, 0x04, 0x02, 
0x3E, 0x51, 0x49, 0x45, 0x3E, 
0x44, 0x42, 0x7F, 0x40, 0x40, 
0x42, 0x61, 0x51, 0x49, 0x46, 
0x21, 0x41, 0x45, 0x4B, 0x31, 
0x18, 0x14, 0x12, 0x7F, 0x10, 
0x27, 0x45, 0x45, 0x45, 0x39, 
0x3C, 0x4A, 0x49, 0x49, 0x30, 
0x01, 0x71, 0x09, 0x05, 0x03, 
0x36, 0x49, 0x49, 0x49, 0x36, 
0x06, 0x49, 0x49, 0x29, 0x1E, 
0x00, 0x6C, 0x6C, 0x00, 0x00, 
0x00, 0xAC, 0x6C, 0x00, 0x00, 
0x08, 0x14, 0x22, 0x41, 0x00, 
0x14, 0x14, 0x14, 0x14, 0x14, 
0x00, 0x41, 0x22, 0x14, 0x08, 
0x02, 0x01, 0x51, 0x09, 0x06, 
0x3E, 0x41, 0x5D, 0x55, 0x5E, 
0x7C, 0x12, 0x11, 0x12, 0x7C, 
0x7F, 0x49, 0x49, 0x49, 0x36, 
0x3E, 0x41, 0x41, 0x41, 0x22, 
0x7F, 0x41, 0x41, 0x22, 0x1C, 
0x7F, 0x49, 0x49, 0x49, 0x41, 
0x7F, 0x09, 0x09, 0x09, 0x01, 
0x3E, 0x41, 0x49, 0x49, 0x7A, 
0x7F, 0x08, 0x08, 0x08, 0x7F, 
0x00, 0x41, 0x7F, 0x41, 0x00, 
0x20, 0x40, 0x41, 0x3F, 0x01, 
0x7F, 0x08, 0x14, 0x22, 0x41, 
0x7F, 0x40, 0x40, 0x40, 0x60, 
0x7F, 0x02, 0x0C, 0x02, 0x7F, 
0x7F, 0x04, 0x08, 0x10, 0x7F, 
0x3E, 0x41, 0x41, 0x41, 0x3E, 
0x7F, 0x09, 0x09, 0x09, 0x06, 
0x3E, 0x41, 0x51, 0x21, 0x5E, 
0x7F, 0x09, 0x19, 0x29, 0x46, 
0x46, 0x49, 0x49, 0x49, 0x31, 
0x03, 0x01, 0x7F, 0x01, 0x03, 
0x3F, 0x40, 0x40, 0x40, 0x3F, 
0x1F, 0x20, 0x40, 0x20, 0x1F, 
0x3F, 0x40, 0x3C, 0x40, 0x3F, 
0x63, 0x14, 0x08, 0x14, 0x63, 
0x07, 0x08, 0x70, 0x08, 0x07, 
0x61, 0x51, 0x49, 0x45, 0x43, 
0x00, 0x7F, 0x41, 0x41, 0x00, 
0x02, 0x04, 0x08, 0x10, 0x20, 
0x00, 0x41, 0x41, 0x7F, 0x00, 
0x04, 0x02, 0x01, 0x02, 0x04, 
0x40, 0x40, 0x40, 0x40, 0x40, 
0x00, 0x01, 0x02, 0x04, 0x00, 
0x20, 0x54, 0x54, 0x54, 0x78, 
0x7F, 0x48, 0x44, 0x44, 0x38, 
0x38, 0x44, 0x44, 0x44, 0x48, 
0x38, 0x44, 0x44, 0x48, 0x7F, 
0x38, 0x54, 0x54, 0x54, 0x18, 
0x08, 0x7E, 0x09, 0x01, 0x02, 
0x08, 0x54, 0x54, 0x58, 0x3C, 
0x7F, 0x08, 0x04, 0x04, 0x78, 
0x00, 0x44, 0x7D, 0x40, 0x00, 
0x20, 0x40, 0x44, 0x3D, 0x00, 
0x7F, 0x10, 0x10, 0x28, 0x44, 
0x00, 0x41, 0x7F, 0x40, 0x00, 
0x7C, 0x04, 0x78, 0x04, 0x78, 
0x7C, 0x08, 0x04, 0x04, 0x78, 
0x38, 0x44, 0x44, 0x44, 0x38, 
0x7C, 0x14, 0x14, 0x14, 0x08, 
0x08, 0x14, 0x14, 0x0C, 0x7C, 
0x7C, 0x08, 0x04, 0x04, 0x08, 
0x48, 0x54, 0x54, 0x54, 0x24, 
0x04, 0x3F, 0x44, 0x40, 0x20, 
0x3C, 0x40, 0x40, 0x20, 0x7C, 
0x1C, 0x20, 0x40, 0x20, 0x1C, 
0x3C, 0x40, 0x38, 0x40, 0x3C, 
0x44, 0x28, 0x10, 0x28, 0x44, 
0x0C, 0x50, 0x50, 0x50, 0x3C, 
0x44, 0x64, 0x54, 0x4C, 0x44, 
0x00, 0x08, 0x36, 0x41, 0x00, 
0x00, 0x00, 0x7F, 0x00, 0x00, 
0x00, 0x41, 0x36, 0x08, 0x00, 
0x02, 0x01, 0x02, 0x04, 0x02, 
0x70, 0x48, 0x44, 0x48, 0x70, 
0x00, 0x0E, 0x11, 0x0E, 0x00, 
0x00, 0x12, 0x1F, 0x10, 0x00, 
0x00, 0x12, 0x19, 0x16, 0x00, 
0x00, 0x11, 0x15, 0x0B, 0x00, 
0x00, 0x07, 0x04, 0x1F, 0x00, 
0x00, 0x17, 0x15, 0x09, 0x00, 
0x00, 0x0E, 0x15, 0x09, 0x00, 
0x00, 0x01, 0x1D, 0x03, 0x00, 
0x00, 0x0A, 0x15, 0x0A, 0x00, 
0x00, 0x12, 0x15, 0x0E, 0x00, 
0x00, 0x04, 0x04, 0x04, 0x00, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0x3E, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x3E, 0x00, 0x00, 0x00, 
0x3E, 0x3E, 0x00, 0x3E, 0x00, 
0x3E, 0x3E, 0x00, 0x3E, 0x3E, 
0x80, 0x80, 0x80, 0x80, 0x80, 
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 
0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 
0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 
0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 
0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 
0x40, 0x00, 0x40, 0x00, 0x40, 
0x60, 0x00, 0x40, 0x00, 0x40, 
0x60, 0x00, 0x70, 0x00, 0x40, 
0x60, 0x00, 0x70, 0x00, 0x78, 
0x7C, 0x00, 0x40, 0x00, 0x40, 
0x7C, 0x00, 0x7E, 0x00, 0x40, 
0x7C, 0x00, 0x7E, 0x00, 0x7F, 
0x1C, 0x77, 0x41, 0x41, 0x41, 
0x41, 0x41, 0x41, 0x41, 0x41, 
0x41, 0x41, 0x41, 0x7F, 0x00, 
0x1C, 0x77, 0x41, 0x5D, 0x5D, 
0x41, 0x41, 0x41, 0x5D, 0x5D, 
0x5D, 0x5D, 0x41, 0x5D, 0x5D, 
0x5D, 0x5D, 0x41, 0x7F, 0x00, 
0x22, 0x1C, 0x14, 0x1C, 0x22, 
0x00, 0x08, 0x1C, 0x08, 0x00, 
0x00, 0x00, 0x77, 0x00, 0x00, 
0x46, 0x5D, 0x55, 0x5D, 0x31, 
0x7C, 0x55, 0x54, 0x55, 0x44, 
0x08, 0x08, 0x2A, 0x08, 0x08, 
0x00, 0x14, 0x08, 0x14, 0x00, 
0x08, 0x14, 0x22, 0x08, 0x14, 
0x7F, 0x41, 0x71, 0x31, 0x1F, 
0x03, 0x05, 0x7F, 0x05, 0x03, 
0x22, 0x14, 0x7F, 0x55, 0x22, 
0x02, 0x55, 0x7D, 0x05, 0x02, 
0x06, 0x09, 0x09, 0x06, 0x00, 
0x44, 0x44, 0x5F, 0x44, 0x44, 
0x1C, 0x14, 0x1C, 0x22, 0x7F, 
0x20, 0x3E, 0x61, 0x3E, 0x20, 
0x20, 0x50, 0x3F, 0x02, 0x0C, 
0x80, 0x7C, 0x20, 0x3C, 0x40, 
0x44, 0x3C, 0x04, 0x7C, 0x44, 
0x00, 0x00, 0x08, 0x00, 0x00, 
0x38, 0x55, 0x54, 0x55, 0x18, 
0x7E, 0x08, 0x10, 0x7F, 0x01, 
0x08, 0x10, 0x08, 0x04, 0x02, 
0x14, 0x08, 0x22, 0x14, 0x08, 
0x0E, 0x06, 0x0A, 0x10, 0x20, 
0x20, 0x10, 0x0A, 0x06, 0x0E, 
0x38, 0x30, 0x28, 0x04, 0x02, 
0x02, 0x04, 0x28, 0x30, 0x38, 
0x7E, 0x11, 0x11, 0x11, 0x7E, 
0x7F, 0x49, 0x49, 0x49, 0x31, 
0x7F, 0x49, 0x49, 0x49, 0x36, 
0x7F, 0x01, 0x01, 0x01, 0x03, 
0xC0, 0x7F, 0x41, 0x7F, 0xC0, 
0x7F, 0x49, 0x49, 0x49, 0x41, 
0x77, 0x08, 0x7F, 0x08, 0x77, 
0x41, 0x49, 0x49, 0x49, 0x36, 
0x7F, 0x10, 0x08, 0x04, 0x7F, 
0x7C, 0x21, 0x12, 0x09, 0x7C, 
0x7F, 0x08, 0x14, 0x22, 0x41, 
0x40, 0x3E, 0x01, 0x01, 0x7F, 
0x7F, 0x02, 0x0C, 0x02, 0x7F, 
0x7F, 0x08, 0x08, 0x08, 0x7F, 
0x3E, 0x41, 0x41, 0x41, 0x3E, 
0x7F, 0x01, 0x01, 0x01, 0x7F, 
0x7F, 0x09, 0x09, 0x09, 0x06, 
0x3E, 0x41, 0x41, 0x41, 0x22, 
0x01, 0x01, 0x7F, 0x01, 0x01, 
0x07, 0x48, 0x48, 0x48, 0x3F, 
0x0E, 0x11, 0x7F, 0x11, 0x0E, 
0x63, 0x14, 0x08, 0x14, 0x63, 
0x7F, 0x40, 0x40, 0x7F, 0xC0, 
0x07, 0x08, 0x08, 0x08, 0x7F, 
0x7F, 0x40, 0x7F, 0x40, 0x7F, 
0x7F, 0x40, 0x7F, 0x40, 0xFF, 
0x01, 0x7F, 0x48, 0x48, 0x30, 
0x7F, 0x48, 0x48, 0x30, 0x7F, 
0x7F, 0x48, 0x48, 0x48, 0x30, 
0x22, 0x41, 0x49, 0x49, 0x3E, 
0x7F, 0x08, 0x3E, 0x41, 0x3E, 
0x46, 0x29, 0x19, 0x09, 0x7F, 
0x20, 0x54, 0x54, 0x54, 0x78, 
0x3C, 0x4A, 0x4A, 0x49, 0x31, 
0x7C, 0x54, 0x54, 0x54, 0x28, 
0x7C, 0x04, 0x04, 0x04, 0x0C, 
0xC0, 0x78, 0x44, 0x7C, 0xC0, 
0x38, 0x54, 0x54, 0x54, 0x18, 
0x6C, 0x10, 0x7C, 0x10, 0x6C, 
0x44, 0x54, 0x54, 0x54, 0x28, 
0x7C, 0x20, 0x10, 0x08, 0x7C, 
0x7C, 0x40, 0x26, 0x10, 0x7C, 
0x7C, 0x10, 0x10, 0x28, 0x44, 
0x40, 0x38, 0x04, 0x04, 0x7C, 
0x7C, 0x08, 0x10, 0x08, 0x7C, 
0x7C, 0x10, 0x10, 0x10, 0x7C, 
0x38, 0x44, 0x44, 0x44, 0x38, 
0x7C, 0x04, 0x04, 0x04, 0x7C, 
0x7C, 0x14, 0x14, 0x14, 0x08, 
0x38, 0x44, 0x44, 0x44, 0x48, 
0x04, 0x04, 0x7C, 0x04, 0x04, 
0x0C, 0x50, 0x50, 0x50, 0x3C, 
0x18, 0x24, 0xFC, 0x24, 0x18, 
0x44, 0x28, 0x10, 0x28, 0x44, 
0x7C, 0x40, 0x40, 0x7C, 0xC0, 
0x0C, 0x10, 0x10, 0x10, 0x7C, 
0x7C, 0x40, 0x7C, 0x40, 0x7C, 
0x7C, 0x40, 0x7C, 0x40, 0xFC, 
0x04, 0x7C, 0x50, 0x50, 0x20, 
0x7C, 0x50, 0x50, 0x20, 0x7C, 
0x7C, 0x50, 0x50, 0x50, 0x20, 
0x28, 0x44, 0x54, 0x54, 0x38, 
0x7C, 0x10, 0x38, 0x44, 0x38, 
0x48, 0x34, 0x14, 0x14, 0x7C
};

#include <Adafruit_ILI9341.h>			// PDQ: Hardware-specific driver library

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#ifdef USE_U8g2
#include <U8g2_for_Adafruit_GFX.h>
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
#else
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#endif


#define ttclk    9  
#define ttcs    6    
#define ttdin   7     
#define ttdout  8     
#define ttirq  12 

#include <URTouch.h>
URTouch  myTouch = URTouch(ttclk, ttcs, ttdin, ttdout, ttirq); //URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);

#include "IDisplay.h"

#define clrBLACK ILI9341_BLACK
#define clrRED ILI9341_RED
#define clrGREEN ILI9341_GREEN
#define clrWHITE ILI9341_WHITE
#define clrBLUE ILI9341_BLUE
#define clrGRAY ConvertRGB(99, 99, 99)
#define clrYELLOW ILI9341_YELLOW

word ConvertRGB( byte R, byte G, byte B)
{
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

class ard_adagfx_ili9341_xpt2046_Display : public IDisplay
{
  bool m_IsTouch;
  short m_TouchX;
  short m_TouchY;

  short _X;
  short _Y;
  short _Color;

public:
	virtual void init()
	{
		delay(200);

		tft.begin();
    tft.setRotation(1);

#ifdef USE_U8g2    
		u8g2_for_adafruit_gfx.begin(tft);
#endif

		myTouch.InitTouch();
		myTouch.setPrecision(PREC_MEDIUM);

    tft.fillScreen(ILI9341_BLACK);
	}

	virtual void update()
	{
    tft.setCursor(0, 0);
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

	virtual void setCursor(short x, short y) override 
	{
    _X = x;
    _Y = y;
	}

	virtual void setTextColor(short color) override 
	{
    _Color = color;
	}

	virtual void print(String txt) override 
	{

#ifdef USE_U8g2
    u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
    u8g2_for_adafruit_gfx.setFont(u8g2_font_10x20_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_for_adafruit_gfx.setForegroundColor(_Color);
    u8g2_for_adafruit_gfx.setCursor(_X, _Y);                // start writing at this position
    u8g2_for_adafruit_gfx.print(txt);
#else
   //tft.setFont(&FreeMonoBoldOblique12pt7b);
   tft.cp437(true);
   tft.setTextSize(2);
   tft.setTextColor(_Color);
   tft.setCursor(_X, _Y);
   //Serial.print("Draw UI: "); Serial.print(txt); Serial.print(" in pos: "); Serial.print(_X); Serial.print(", ");Serial.print(_Y);Serial.println("");
   tft.print(utf8rus(txt));
#endif    
	}

  virtual void drawLine(short x1, short y1, short x2, short y2, short color)
  {
    tft.drawLine(x1, y1, x2, y2, color);
  }

  virtual void drawVLine(uint16_t  x0, uint16_t  y0, uint16_t len, uint16_t color)
  {
    tft.drawFastVLine(x0, y0, len, color);
  }

  virtual void fillRect(short x1, short y1, short x2, short y2, short color)
  {
    tft.fillRect(x1, y1, x2, y2, color);
  }

	virtual bool isTouch() override 
	{
		return m_IsTouch;
	}

	virtual short getTouchX() override 
	{
		return m_TouchX;
	}

	virtual short getTouchY() override 
	{
		return m_TouchY;
	}

 /* Recode russian fonts from UTF-8 to Windows-1251 */
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
};



#endif
