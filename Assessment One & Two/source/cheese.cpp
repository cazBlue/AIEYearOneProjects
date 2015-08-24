#include "cheese.h"
#include "mouse.h"


Cheese::Cheese(Level *lvl, DrawEngine *de, int s_index, float x, float y, int i_lives) : Sprite(lvl, de, s_index, x, y, i_lives)
{
	//default constructor

//	eatenBy = 0;	//fill with default value to avoid any errors/clear memory out

	classID = CHEESE_CLASSID;

	showCheese();	//	draw the cheese to the screen

}


void Cheese::showCheese(void)
{
	//the cheese is generated in level.cpp this function merely draws it to the screen
	draw(pos.x, pos.y);	
}



bool Cheese::setEatenBy(Mouse mouse)
{
	//set which mouse ate the cheese!
	return false;
}

