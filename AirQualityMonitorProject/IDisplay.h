#ifndef IDisplay_h
#define IDisplay_h

class IDisplay
{
public:  
  void init();
	void setCursor(short x, short y);
	void setTextColor(short x);
	void print(String txt);

	bool isTouch();
	short getTouchX();
	short getTouchY();
};


#endif
