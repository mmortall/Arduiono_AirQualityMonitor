#ifndef MeasurementsScreen_H
#define MeasurementsScreen_H

#include "UIElement.h"

class MeasurementsScreen
{
public:
  UIElement* CO2Btn;
  UIElement* CO2Val;
  UIElement* TemperBtn;
  UIElement* TemperVal;
  UIElement* PresBtn;
  UIElement* PresVal;
  UIElement* DustBtn;
  UIElement* DustVal;
      
  MeasurementsScreen()
  {
    short yPos = 15;
    short yOffset = 20;
    short xValPos = 200;
    CO2Btn = new UIElement("Концентр. CO2: ", 0, yPos, 80, 40); 
    CO2Val = new UIElement("1234 ppm", xValPos, yPos, 80, 40, clrRED); 
    yPos += yOffset;
    TemperBtn = new UIElement("Температура: ", 0, yPos, 80, 40); 
    TemperVal = new UIElement("25.2", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
    PresBtn = new UIElement("Атмос. давление: ", 0, yPos, 80, 40); 
    PresVal = new UIElement("-1", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
    DustBtn = new UIElement("Пыль (частицы): ", 0, yPos, 80, 40); 
    DustVal = new UIElement("1234 ppm", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
  }

	void init() 
	{
	};

	void update(IDisplay& render) 
	{
    CO2Btn->render(render);
    CO2Val->render(render);
    TemperBtn->render(render);
    TemperVal->render(render);
    PresBtn->render(render);
    PresVal->render(render);
    DustBtn->render(render);
    DustVal->render(render);
	};

	short getTouchCountrol(short x, short y)
	{
		return 0;
	}
};

#endif
