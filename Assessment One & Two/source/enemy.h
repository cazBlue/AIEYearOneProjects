#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

class Level;	//avoid cyclic error
class Character; //avoid cyclic error

class Enemy : public Sprite
{
public:
	Enemy(Level *l, DrawEngine *de, int s_index, float x = 1, float y = 1, int i_lives = 1);

	void addGoal(Character *g);		//pass reference to the desired goal, will follow a moving goal

	bool move(float x, float y);	//inherited function, adds AI movment to enemy
	
	virtual void idleUpdate(void);	//if the enemy has a current goal move towards the player

protected:
	virtual void simulateAI(void);			//will move towards a set goal, called by idle update -- verbose method
	Character *goal;				//current goal of enemy
};

#endif