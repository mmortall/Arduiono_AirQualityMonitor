
#ifdef USE_ADAFRUT_DIPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);
#endif

#ifdef USE_TOUCH_DIPLAY
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_CS         2
#define TFT_RST        3 
#define TFT_DC         4

#define TFT_CLK        13
#define TFT_MISO       10 
#define TFT_MOSI       11

Adafruit_ILI9341 TFTscreen = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#endif
unsigned short _CurrDiplayCursorX;
unsigned short _CurrDiplayCursorY;

void ClearDisplay()
{
#ifdef USE_ADAFRUT_DIPLAY  
  display.clearDisplay();
#else
  TFTscreen.fillScreen(ILI9341_BLACK);
#endif  
}

#ifdef USE_ADAFRUT_DIPLAY
#define DisplayPrint(val) display.print(val)
#else
#define DisplayPrint(val) TFTscreen.print(String(val).c_str())
#endif    

void SetTextColor(int color)
{
#ifdef USE_ADAFRUT_DIPLAY  
  display.setTextColor(color);
#else
  TFTscreen.setTextColor(color);
#endif  
}

void DisplaySetCursor(int x, int y)
{
  _CurrDiplayCursorX = x;
  _CurrDiplayCursorY = y;
#ifdef USE_ADAFRUT_DIPLAY  
  display.setCursor(x,y);
#else

#endif    
}

void DisplayRender()
{
#ifdef USE_ADAFRUT_DIPLAY   
  display.display();
#endif    
}
  
