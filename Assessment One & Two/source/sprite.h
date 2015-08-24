#ifndef SPRITE_H
#define SPRITE_H

#include "drawEngine.h"
#include "level.h"

/*
------------------------------------------------
Sprite class is the parent class for anything that needs to draw to the screen

has access to the draw engine


------------------------------------------------
*/


//enum containing sprite classes
enum 
{
	SPRITE_CLASSID,
	FIREBALL_CLASSID,
	CHARACTER_CLASSID,
	ENEMY_CLASSID,
	MAGE_CLASSID,
	MOUSE_CLASSID,
	CHEESE_CLASSID
};

//class Level;

//custom struct to create 2d vector
struct vector
{
	float x;
	float y;
};

class Sprite
{
public:	
	Sprite(Level *level, DrawEngine *de, int s_index, float x = 1, float y = 1, int lives = 1);//default constructor	
	~Sprite();										//destructor
	
	vector getPosition(void);						//get the players position, returns custom vector struct

													//get players x and y position
	float getX(void);
	float getY(void);
	
	virtual void addLives(int num = 1);				//virtual function for overriding in inherited functions - adds lives to the player
	int getLives(void);								//get how many lives the sprite has
	bool isAlive(void);								// simple check to see if the player is alive
	
	virtual bool move(float x, float y);			//overideable function that moves the player to passed x,y position.

	virtual void idleUpdate(void);

	int classID;									//what kind of sprite is this?
	
	void draw(float x, float y);					//draw the sprite at locatin

protected:

	Level *level;									//make Sprite class aware of the level

	DrawEngine *drawArea;							//pointer as we may not know what all the settings will be at launch
													//location of sprite on screen
	vector pos;
													//index of THIS sprite within sprite array
	int spriteIndex;
													
	int numLives;									//how many lives do we have
													
	vector facingDirection;							//direction of sprite, set in default constructor												
													
	void erase(float x, float y);					//remove the sprite at location
	
	bool isValidLevelMove(int xpos, int ypos);		//checks if the desired move location is taken or not

private:
};

#endif