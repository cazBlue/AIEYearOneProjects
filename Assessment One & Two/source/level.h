#ifndef LEVEL_H
#define LEVEL_H

#include "drawEngine.h"
#include <list>

using std::list;
//#include "character.h"

//define the type of sprite to display
enum SPRITE_ID
{
	SPRITE_PLAYER,
	SPRITE_ENEMY,
	SPRITE_FIREBALL,
	SPRITE_MOUSE,
	SPRITE_CHEESE
};

enum TILE_ID// define status of tiles
{
	TILE_EMPTY,
	TILE_WALL
};

class Sprite;
class Character; //pre-defining to avoid cyclic loop, that thing is messed up

//level class controls the generation of the level
class Level
{
public:
	Level(DrawEngine *de, int width = 30, int height = 20);	//default constructor level dimensions, takes reference to the draw engine.
	~Level();

	void addPlayer(Character *p);	//pass in pointer to the player and will assign player to empty level pointer player
	void update(void);				//update AI and other non human assets
	void draw(void);				//pushes the draw command through to the draw engine, also draws the background
	bool keyPress(char c);			// main game loop passes key pressess to the level to handle

	void addEnemies(int num);		//pass number of enemies to create at random in the level
	void addMouse(int num);			//pass number of mousies to create at random in the level
	void addCheese(int num);
	
	void drawAllNpc();

	int countMice();

	friend class Sprite;			//set this class to a friend of the sprite class, allowing that class to access the private variables and functions
	friend class Mage;			//set this class to a friend of the sprite class, allowing that class to access the private variables and functions
	friend class Mouse;			//set this class to a friend of the sprite class, allowing that class to access the private variables and functions

private:
	int width, height;				//width and height of the level
	int HUDRedraw;					//counter to stop HUD redraw every frame

	char **gameLevel;				//2 dimensional pointer array of the game level

	//draw/update the players HUD
	void drawHud(int score, int lives, int cheeseLeft, int miceActive);
	
	Character *player;				//reference to the current player, defined in the main game loop
	DrawEngine *levelDrawArea;		//reference to the game engines draw area

protected:
	void createLevel();				//loops through the width and height to generate the 2 dimensional game level array. stores in **game level

	void addNPC(Sprite *spr);		//called by addEnemies, adds sprite to the npc Sprite List

public:
	list <Sprite *> npc;			//list of npc sprites
	list <Sprite *>::iterator Iter;	//iter for use on the npc list
};


#endif