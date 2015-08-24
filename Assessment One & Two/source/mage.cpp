#include "mage.h"
#include "fireball.h"
#include "level.h"

Mage::Mage(Level *level, DrawEngine *de, int s_index, float x, float y , int lives, char spell_key,  char up_key, char down_key, char left_key, char right_key) 
			: Character(level, de, s_index, x, y, lives, up_key, down_key, left_key, right_key)
{
	spellKey = spell_key;
	classID = MAGE_CLASSID;
	score = 0;
}



bool Mage::keyPress(char c)
{
	//returns false if not a movement key
	bool val = Character::keyPress(c);

	//if it's not a movement key check if it's a spell cast
	if(!val)
	{
		if (c == spellKey)
		{
			castSpell();
			return true;
		}
	}
	
	
	if(Mage::checkForCheese())
	{	
		//increase the players score
		addScore(20);
	}
	
	return val;
}

bool Mage::checkForCheese()
{
	list <Sprite *>::iterator Iter;

	for (Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
	{
		if ((*Iter)->classID == CHEESE_CLASSID &&  int((*Iter)->getX()) == int(pos.x) && int((*Iter)->getY()) == int(pos.y))
		{
			//destroy the cheese
			(*Iter)->addLives(-1);

			return true;
		}
	}

	return false;
}

void Mage::castSpell(void)
{
	//the problem with the original is that it can cast into walls
	//updated to check is the cast position would be a valid move for the player

	vector newMove;
	newMove.x = (int)pos.x + facingDirection.x;
	newMove.y = (int)pos.y + facingDirection.y;

	if(isValidLevelMove(newMove.x, newMove.y))
	{
		Fireball *temp = new Fireball(level, drawArea, SPRITE_FIREBALL, 
		(int)pos.x + facingDirection.x, (int)pos.y + facingDirection.y, 
		facingDirection.x, facingDirection.y);

		level->addNPC((Sprite *) temp);
	}
}

bool Mage::move(float x, float y)
{
	Sprite::move( x, y);

	//to do 
	//add checks for powerups/cheese
	
	level->gameLevel[int(x)][int(y)];



	return false;
}



bool Mage::addPowerUp(int PowerUp)
{

	int temp;

	return false;
}

bool Mage::checkGameOver(int *arg_endtype)
{
	if(getLives() <= 0)
	{
		//TODO 
		//add in return to high score screen
		
		//TODO go to lose screen
		*arg_endtype = ENDTYPE_LOSE;
		

		return true;
	}
	

	if(getMouseCount() <= 0 && getLives() > 0)
	{
		//TODO go to win screen
		*arg_endtype = ENDTYPE_WIN;

		return true;
	}
	else if(getMouseCount() <= 0)
	{
		*arg_endtype = ENDTYPE_LOSE;

		return true;
	}


	return false;
}

int Mage::getMouseCount()
{
	return level->countMice();
}


void Mage::addLives(int arg_lives)
{
	/*
	overrides the character addlives as we don't want to reset the players position
	call the sprites function as it meets the current need
	*/
	Sprite::addLives(arg_lives);
	//checkGameOver();
}

int Mage::getScore()
{
	return score;
}

void Mage::addScore(int arg_scoreToAdd)
{
	score += arg_scoreToAdd;
}