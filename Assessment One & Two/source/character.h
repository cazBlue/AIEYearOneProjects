#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"

/*---------------------------------
character class inherits from sprite class and adds functioniality to allow the player to take input to move within the level.

actual movement functioniality is inherited from the sprite class

----------------------------------*/

//character class inherits sprite class
class Character : public Sprite
{
public:
	//default constructor
	Character(Level *level, DrawEngine *de, int s_index, float x = 1, float y = 1, int lives = 3, char up_key = 'w', char down_key = 's', char left_key = 'a', char right_key = 'd');

	//overridable keypress function
	virtual bool keyPress(char c);

	//overrides sprite add lives function and remains overidable
	virtual void addLives(int num = 1);

protected:
	//current keys being used for player control
	char upKey, downKey, leftKey, rightKey;

private:

};

#endif