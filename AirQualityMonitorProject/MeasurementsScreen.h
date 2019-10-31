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
  UIElement* HumidityBtn;
  UIElement* HumidityVal;
#ifdef USE_DUST_SENSOR  
  UIElement* DustBtn;
  UIElement* DustVal;
#endif
      
  MeasurementsScreen()
  {
    short yPos = 15;
    short yOffset = 20;
    short xStartPos = 1;
    short xValPos = 190;
    CO2Btn = new UIElement("Концентр. CO2: ", xStartPos, yPos, 80, 40); 
    CO2Val = new UIElement("1234 ppm", xValPos, yPos, 80, 40, clrRED); 
    yPos += yOffset;
    TemperBtn = new UIElement("Температура: ", xStartPos, yPos, 80, 40); 
    TemperVal = new UIElement("25.2", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
    PresBtn = new UIElement("Атмос. давление: ", xStartPos, yPos, 80, 40); 
    PresVal = new UIElement("-1", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
    HumidityBtn = new UIElement("Влажность: ", xStartPos, yPos, 80, 40); 
    HumidityVal = new UIElement("-1", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
#ifdef USE_DUST_SENSOR    
    DustBtn = new UIElement("Пыль (частицы): ", xStartPos, yPos, 80, 40); 
    DustVal = new UIElement("1234 ppm", xValPos, yPos, 80, 40, clrGREEN); 
    yPos += yOffset;
#endif    
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
    HumidityBtn->render(render);
    HumidityVal->render(render);
#ifdef USE_DUST_SENSOR       
    DustBtn->render(render);
    DustVal->render(render);
#endif     
	};

	short getTouchCountrol(short x, short y)
	{
		return 0;
	}
};

#endif
