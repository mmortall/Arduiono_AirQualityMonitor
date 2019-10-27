#ifndef TouchArea_H
#define TouchArea_H

class TouchArea
{
public:
  TouchArea(short x, short y, short w, short h)
  {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;            
  }

	bool IsPointInArea(short x, short y)
	{
		if (x >= this->x && y >= this->y
			&& x <= this->w && y <= this->h)
		{
			return true;
		}

		return false;
	}
protected:
  short x;
  short y;
  short w;
  short h;

};

#endif
