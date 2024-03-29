// This is part of the PDQ re-mixed version of Adafruit's GFX library
// and associated chipset drivers.
// Here is the libraries original copyright notice:

/***************************************************
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

 
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>				// must include this here (or else IDE can't find it) 
                                           
#include <Adafruit_GFX.h>				// PDQ: Core graphics library
#include "PDQ_ILI9341_config.h"			// PDQ: ST7735 pins and other setup for this sketch
#include <Adafruit_ILI9341.h>			// PDQ: Hardware-specific driver library
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

#define ttclk    9  
#define ttcs    6    
#define ttdin   7     
#define ttdout  8     
#define ttirq  12 

#include <URTouch.h>
URTouch  myTouch = URTouch( ttclk, ttcs, ttdin, ttdout, ttirq); //URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);

// NOTE: Changes to test with Adafruit libraries (comment out PDQ lines above and un-comment the AF: ones below)

// AF: #include <Adafruit_GFX.h>		// AF: Core graphics library
// AF: #include <Adafruit_ILI9341.h>		// AF: Hardware-specific library
// AF: Adafruit_ILI9341 tft(10,  9, 8); 	// AF: create LCD object (HW SPI, CS=pin 10, D/C=pin 8, reset=9)

extern "C" char __data_start[];		// start of SRAM data
extern "C" char _end[];			// end of SRAM data (used to check amount of SRAM this program's variables uses)
extern "C" char __data_load_end[];	// end of FLASH (used to check amount of Flash this program and static data uses)

SoftwareSerial mySerial(A0, A1); 

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  
  delay(200);
/*  
//	OSCCAL = 0xae;	// ~20MHz ATtiny overclock
	OSCCAL = 0xd0;	// ~25MHz ATtiny overclock
	OSCCAL = 0xf2;	// ~30MHz ATtiny overclock
#if defined(ILI9341_RST_PIN)	// reset like Adafruit does
	FastPin<ILI9341_RST_PIN>::setOutput();
	FastPin<ILI9341_RST_PIN>::hi();
	FastPin<ILI9341_RST_PIN>::lo();
	delay(1);
	FastPin<ILI9341_RST_PIN>::hi();
#endif*/

	tft.begin();
  u8g2_for_adafruit_gfx.begin(tft); 
  tft.setRotation(1);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
}

void loop(void)
{
	/*uint32_t usecLines = testLines(ILI9341_BLUE);
	delay(100);

	uint32_t usecFillScreen = testFillScreen();
	delay(100);

	uint16_t c = 4;
	int8_t d = 1;
	for (int32_t i = 0; i < tft.height(); i++)
	{
		tft.drawFastHLine(0, i, tft.width(), c);
		c += d;
		if (c <= 4 || c >= 11)
			d = -d;
	}*/
 
 tft.fillScreen(ILI9341_BLACK);
 tft.setCursor(0, 0);

/*	tft.setCursor(0, 0);
	tft.setTextColor(ILI9341_MAGENTA);
	tft.setTextSize(2);

#if defined(_PDQ_ILI9341H_)
	tft.print(F("PDQ GFX+PDQ ILI9341 "));
#else
 	tft.print(F("Adafruit GFX+ILI9341"));
#endif
	tft.setTextSize(1);
	tft.setTextColor(ILI9341_WHITE);
	tft.println(F(" SPI LCD Benchmark (ATtiny85 @ 16MHz)"));
	tft.setTextSize(1);
	tft.println(F(""));
	tft.setTextColor(tft.color565(0x80, 0x80, 0x80));
	tft.print(F("  Memory Used:  Flash=0x"));
	tft.print((uint16_t)__data_load_end, HEX);
	tft.print(F(" RAM=0x"));
	tft.println((uint16_t)_end - (uint16_t)__data_start, HEX);
	tft.println(F(""));
	tft.print(F(" OSCCAL=0x"));
	tft.println(OSCCAL, HEX);
	tft.println(F(""));
	tft.println(F(""));

	tft.setTextColor(ILI9341_GREEN);
	tft.println(F(" Benchmark               microseconds"));
	tft.println(F(""));

	tft.setTextColor(ILI9341_CYAN); tft.setTextSize(1);
	tft.print(F("Lines              "));
	tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
	//printnice(usecLines);

	tft.setTextColor(ILI9341_CYAN); tft.setTextSize(1);
	tft.print(F("Screen fill        "));
	tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
//	printnice(usecFillScreen);

	tft.setTextSize(1);
	tft.println(F(""));
	tft.setTextColor(ILI9341_GREEN); tft.setTextSize(2);
	tft.print(F("Benchmark Completed!"));
 tft.setTextSize(0);
  tft.println(utf8rus("Тест вывод текста на русском"));
*/
tft.println(F(""));
  //tft.clearDisplay();                               // clear the graphcis buffer  
  u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_WHITE);      // apply Adafruit GFX color
  u8g2_for_adafruit_gfx.setFont(u8g2_font_10x20_t_cyrillic);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2_for_adafruit_gfx.setCursor(0,20);                // start writing at this position
  u8g2_for_adafruit_gfx.print(F("Hello Русский Текст"));
  return;

  //test touch
  u8g2_for_adafruit_gfx.println();
  if(myTouch.dataAvailable() == true)
  {
    myTouch.read();
    u8g2_for_adafruit_gfx.print("Touch X:"); u8g2_for_adafruit_gfx.print(myTouch.getX());u8g2_for_adafruit_gfx.println(F(""));
    u8g2_for_adafruit_gfx.print("Touch Y:"); u8g2_for_adafruit_gfx.print(myTouch.getY());u8g2_for_adafruit_gfx.println(F(""));
  }
  else
  {
     u8g2_for_adafruit_gfx.println(F("NO touch"));
  }
  
	delay(200L);
}

void printnice(int32_t v)
{
	char	str[32] = { 0 };
	sprintf(str, "%lu", v);
	for (char *p = (str+strlen(str))-3; p > str; p -= 3)
	{
		memmove(p+1, p, strlen(p)+1);
		*p = ',';
		
	}
	while (strlen(str) < 10)
	{
		memmove(str+1, str, strlen(str)+1);
		*str = ' ';
	}
	tft.print(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
	uint8_t oms = millis();
	while ((uint8_t)millis() == oms)
		;
	return micros();
}

uint32_t testLines(uint16_t color)
{
	uint32_t start, t;
	int32_t	x1, y1, x2, y2;
	int32_t	w = tft.width();
	int32_t	h = tft.height();

	tft.fillScreen(ILI9341_BLACK);

	x1 = y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = w - 1;

	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t = micros() - start; // fillScreen doesn't count against timing

	tft.fillScreen(ILI9341_BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	tft.fillScreen(ILI9341_BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	t += micros() - start;

	tft.fillScreen(ILI9341_BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	return t;
}
uint32_t testFillScreen()
{
	uint32_t start = micros_start();

	for (uint8_t i = 0; i < 30; i++)
	{
		tft.fillScreen(ILI9341_RED);
		tft.fillScreen(ILI9341_BLACK);
	}

	return micros() - start;
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
    m[0] = n; 
    target = target + String(m);
  }
  return target;
}
