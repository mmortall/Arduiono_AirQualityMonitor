
#ifdef USE_ADAFRUT_DIPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);
#endif

#ifdef USE_TOUCH_DIPLAY
#include <SPI.h>
#include <TFT.h>
#endif

#ifdef USE_ADAFRUT_DIPLAY
#define DisplayPrint(val) display.print(val)
#else
#define DisplayPrint(val) TFTscreen.text(val, _CurrDiplayCursorX, _CurrDiplayCursorY)
#endif    


#ifdef USE_TOUCH_DIPLAY
#define SCLK (4)
#define MOSI (5)
#define CS   (6)
#define DC   (7)
#define RESET (8) 
TFT TFTscreen = TFT(CS, DC, MOSI);
#endif

unsigned short _CurrDiplayCursorX;
unsigned short _CurrDiplayCursorY;

void ClearDisplay()
{
#ifdef USE_ADAFRUT_DIPLAY  
  display.clearDisplay();
#else

#endif  
}

void SetTextColor(int color)
{
#ifdef USE_ADAFRUT_DIPLAY  
  display.setTextColor(color);
#else

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
  
