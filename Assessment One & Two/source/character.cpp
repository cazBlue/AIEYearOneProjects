#include "character.h"

//character default constructor expands sprite default constructor
Character::Character(Level *level, DrawEngine *de, int s_index, float x, float y, int lives, char u, char d, char l, char r) : Sprite(level, de, s_index, x, y, lives)
{
	//tell the player what the controls are, not harcoded so we can expand the game for say multiplayer
	upKey = u;
	downKey = d;
	leftKey = l;
	rightKey = r;

	// set the classid of the character to CHARACTER_CLASSID for easy reference to type
	classID = CHARACTER_CLASSID;		//set in Sprite.h
}

//but the character around, controls direction coords - windows console is 0,0 from the top left
bool Character::keyPress(char c)
{
	if (c == upKey)
	{
		return move(0, -1);
	}
	else if (c == downKey)
	{
		return move(0, 1);
	}
	else if (c == leftKey)
	{
		return move(-1, 0);
	}
	else if (c == rightKey)
	{
		return move(1, 0);
	}

	// if the move fails return that we did not move
	return false;
}

void Character::addLives(int num)
{
	Sprite::addLives(num);	//use Sprites add lives function
	
	if (Sprite::isAlive()) //if the player still has lives set the player back to start
	{
		pos.x = 1;
		pos.y = 1;
		move(0,0);
	}
	else
	{
		
	}
}