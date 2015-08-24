#include "sprite.h"

//sprite 
Sprite::Sprite(Level *l, DrawEngine *de, int s_index, float x, float y , int lives)
{
	drawArea = de;
	
	//set the default location of the sprite
	pos.x = x;
	pos.y = y;
	//set the sprite index, must be unique to confusion
	spriteIndex = s_index;

	//set the players lives
	numLives = lives;

	//set the default facing direction to right
	facingDirection.x = 1;
	facingDirection.y = 0;

	level = l;
	

	classID =  SPRITE_CLASSID;

	
}

Sprite::~Sprite()
{
	// erase the dying sprite
	erase(pos.x, pos.y);
}

vector Sprite::getPosition(void)
{
	return pos;
}

float Sprite::getX(void)
{
	return pos.x;
}

float Sprite::getY(void)
{
	return pos.y;
}

void Sprite::addLives(int num)
{
	numLives += num;
}

int Sprite::getLives(void)
{
	return numLives;
}

bool Sprite::isAlive(void)
{	
	//if(numLives <= 0)
	//	erase(pos.x, pos.y);

	return (numLives > 0);
}

bool Sprite::move(float x, float y)
{
	int xpos = (int)(pos.x + x);
	int ypos = (int)(pos.y + y);

	if(isValidLevelMove(xpos, ypos))
	{

		//erase sprite at current location
		erase(pos.x, pos.y);

		//move the sprite to the new cords
		pos.x += x;
		pos.y += y;

		//set the facing direction
		facingDirection.x = x;
		facingDirection.y = y;
	
		//draw sprite at new location
		draw(pos.x, pos.y);
		
		return true;
	}

	//update once level is in place
	return false;
}

void Sprite::draw(float x, float y)
{
	drawArea->drawSprite(spriteIndex, (int)x, (int)y);
}

void Sprite::erase(float x, float y)
{
	drawArea->eraseSprite((int)x, (int)y);
}

bool Sprite::isValidLevelMove(int xpos, int ypos)
{
	if(level->gameLevel[xpos][ypos] != TILE_WALL)	//access the game level array to see which tile is in place
		return true;
	
	return false;
}

void Sprite::idleUpdate(void)
{
	// this is for the in
}