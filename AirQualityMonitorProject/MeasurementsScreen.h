#ifndef MeasurementsScreen_H
#define MeasurementsScreen_H

#include "UIElement.h"

class MeasurementsScreen
{
public:
	void init(IDisplay render) 
	{
    _Render = render;
		_CO2Btn = UIElement("Концентрация CO2: ", 10, 10, 80, 40);
	};

	void update() 
	{
    _CO2Btn.render(_Render);
	};

	short getTouchCountrol(short x, short y)
	{
		return 0;
	}

private:
  IDisplay _Render;
	UIElement _CO2Btn;
};

#endif
