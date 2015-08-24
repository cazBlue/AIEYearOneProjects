#ifndef CHEESE_H
#define CHEESE_H

#include "sprite.h"
//#include "mouse.h"

class Mouse;	//declare mouse without include to avoid cyclic error

class Cheese : public Sprite
{
public:
	Cheese(Level *l, DrawEngine *de, int s_index, float x = 1, float y = 1, int i_lives = 1);

	bool setEatenBy(Mouse mouse);

	

protected:
//	Mouse *eatenBy;

	void showCheese(void);

private:


};


#endif