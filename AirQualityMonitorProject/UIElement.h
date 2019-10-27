#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "TouchArea.h"
#include "IDisplay.h"

class UIElement: public TouchArea
{
public:
	UIElement(const char* label, short x, short y, short w, short h, bool drawBG = false, bool drawRect = false): TouchArea(x,y,w,h)
	{
    setLabel(label);
    m_LabelPrev = label;
    m_DrawText = true;
    m_DrawBG = drawBG;
    m_DrawRect = drawRect;    
	}

  UIElement(short x, short y, short w, short h, bool drawBG = false, bool drawRect = false): TouchArea(x,y,w,h)
  {
    setLabel(NULL);
    m_LabelPrev = NULL;
    m_DrawText = false;
    m_DrawBG = drawBG;
    m_DrawRect = drawRect;    
  }

	void render(IDisplay tft)
	{
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
			tft.print(m_LabelPrev);

			tft.setTextColor(TextColor);
			tft.print(m_Label);
		}
	}

 bool itPressed(IDisplay tft)
 {
    return tft.isTouch() && IsPointInArea(tft.getTouchX(), tft.getTouchY());
 }

	void setLabel(const char* label)
	{
		m_LabelPrev = m_Label;
		m_Label = label;
	}

private:
	const char* m_Label;
	const char* m_LabelPrev;
	int TextColor;
	int BGColor;
	int RectColor;

	bool m_DrawText;
	bool m_DrawBG;
	bool m_DrawRect;
};

#endif
