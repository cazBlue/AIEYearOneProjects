#ifndef MOUSE_H
#define	MOUSE_H

#include "enemy.h"

class Cheese;
//class Level;	//avoid cyclic error
//class Character; //avoid cyclic error

class Mouse : public Enemy
{
public:
	Mouse(Level *l, DrawEngine *de, int s_index, float x = 1, float y = 1, int i_lives = 1, int smellDistance = 10);	

	//called idle update as it only does things if the npc has nothing to do!
	void idleUpdate(void);	//Mouse will search for cheese until it's within smell distance, then home in.


	void simulateAI(void);	//mouse will search for cheese

	bool eatenCheese(void);

	virtual void addLives(int num);

	void reDrawMouse(void);
	

protected:

	int mouseSmellDistance;

	bool hasCheese;
	bool smellsCheese;

	Sprite *targetCheese;

	vector cheesePos;

private:


};



#endif