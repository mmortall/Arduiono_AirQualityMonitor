#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "TouchArea.h"
#include "IDisplay.h"

class UIElement: public TouchArea
{
public:
	UIElement(const char* label, short x, short y, short w, short h, short color = clrWHITE, bool drawBG = false, bool drawRect = false): TouchArea(x,y,w,h), m_IsDirty(true)
	{
    setLabel(label);
    m_LabelPrev = label;
    m_DrawText = true;
    m_DrawBG = drawBG;
    m_DrawRect = drawRect; 
    TextColor = color;
	}

  UIElement(short x, short y, short w, short h, short color = clrWHITE, bool drawBG = false, bool drawRect = false): TouchArea(x,y,w,h), m_IsDirty(true)
  {
    m_DrawText = false;
    m_DrawBG = drawBG;
    m_DrawRect = drawRect;   
    TextColor = color;
  }

	void render(IDisplay& tft)
	{
    if(m_IsDirty)
       m_IsDirty = false;
    else
      return;
    
		if (m_DrawBG)
		{
		}

		if (m_DrawRect)
		{

		}
		if (m_DrawText)
		{
			//clean
			tft.setCursor(x, y);
			tft.setTextColor(clrBLACK);
      if(m_LabelPrev != NULL)
			  tft.print(m_LabelPrev.c_str());

			tft.setTextColor(TextColor);
      if(m_Label != NULL)
			  tft.print(m_Label.c_str());
		}
	}

  bool isPressed(IDisplay& tft)
 {
    return tft.isTouch() && IsPointInArea(tft.getTouchX(), tft.getTouchY());
 }

	void setLabel(const char* label)
	{
    if(strcmp(label, m_Label.c_str()) == 0)
      return;
    
    m_LabelPrev = m_Label;
    m_Label = label;
    m_IsDirty = true;
	}

private:
	String m_Label;
	String m_LabelPrev;
	int TextColor;
	int BGColor;
	int RectColor;

	bool m_DrawText;
	bool m_DrawBG;
	bool m_DrawRect;

  bool m_IsDirty;
};

#endif
