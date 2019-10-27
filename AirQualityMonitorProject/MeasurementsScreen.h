#ifndef MeasurementsScreen_H
#define MeasurementsScreen_H

#include "UIElement.h"

class MeasurementsScreen
{
public:
  UIElement* CO2Btn;

  MeasurementsScreen()
  {
    CO2Btn = new UIElement("Концентрация CO2: ", 0, 15, 80, 40);
  }

	void init() 
	{
	};

	void update(IDisplay& render) 
	{
    CO2Btn->render(render);
	};

	short getTouchCountrol(short x, short y)
	{
		return 0;
	}
};

#endif
