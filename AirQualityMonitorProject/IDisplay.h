#ifndef IDisplay_h
#define IDisplay_h

class IDisplay
{
public:  
  virtual void init();
	virtual void setCursor(short x, short y) = 0;
	virtual void setTextColor(short x) = 0;
	virtual void print(String txt) = 0;

  virtual void drawLine(short x1, short y1, short x2, short y2, short color) = 0;
  virtual void fillRect(short x1, short y1, short x2, short y2, short color) = 0;
  
	virtual bool isTouch() = 0;
	virtual short getTouchX() = 0;
	virtual short getTouchY() = 0;
};


#endif
