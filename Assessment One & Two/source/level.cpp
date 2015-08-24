#include "level.h"
#include "stdlib.h"
#include "character.h"
#include "enemy.h"
#include "mage.h"
#include "mouse.h"
#include "cheese.h"
#include <sstream>

#include <iostream>	//used for debugging only

Level::Level(DrawEngine *de, int w, int h)
{
	//get reference to the draw engine
	levelDrawArea = de;

	//set the draw dimensions to match the constructor dimensions
	levelDrawArea->setScreenDimensions(w, h);


	width = w;
	height = h;
	HUDRedraw = 50; //force hud redraw on launch
	//currently we do not have a player, init it to 0
	player = 0;

	//--------------------- create memory for our level
	gameLevel = new char *[width];

	//for every char[width] index assign the height
	//this sets up an empty 2 dimensional array to the size of our defined width and height
	for (int x = 0; x < width; x++)
	{
		gameLevel[x] = new char[height];
	}

	//---------------end level memory creation

	//create the level
	createLevel();

	//set the map for the level
	levelDrawArea->setMap(gameLevel);

}

Level::~Level()
{
	//cleans all npc's out of the level. npc's are pointers and need to be cleaned
	for (Iter = npc.begin(); Iter != npc.end(); Iter++)
	{
		delete (*Iter);
	}


	//delete the individual level pointers
	for (int x = 0; x < width; x++)
		delete [] gameLevel[x];

	//delete the game level, (game level is an array)
	delete [] gameLevel;
}

//loops through the width and height to generate the 2 dimensional game level array. stores in **game level
void Level::createLevel(void)
{
	for (int x = 0; x < width; x ++)	//for every row of width loop
	{
		for (int y = 0; y < height; y++) //for every row of width loop through ever row or height, then move back to the next width row in the initial for loop
		{
			int random = rand() % 100;	//who doesn't love a random number of modulo 100

			if( y == 0 || y == height - 1 || x == 0 || x == width - 1)	//is this the outside wall, if so set it all to walls
			{
				gameLevel[x][y] = TILE_WALL;
			}
			else
			{
				//randomly select if tiles should be walls or empty
				if(random < 90 || (x < 3 && y < 3))// hard coded 90% chance not to be walls - also ensures that the players start point always has empty tiles around the start point of 1,1
				{
					gameLevel[x][y] = TILE_EMPTY;
				}
				else
				{
					gameLevel[x][y] = TILE_WALL;
				}
			}
		}
	}
}

void Level::draw(void)
{
	levelDrawArea->drawBackground();
}

void Level::addPlayer(Character *p)
{
	player = p;
}

bool Level::keyPress(char c)
{
	if(player)
	{
		if(player->keyPress(c))
		{
			return true;
		}
	}

	
	return false;
}

void Level::update(void)
{
	//stat tracking	
	int mice = 0, cheese = 0;
	
	//here is AI and anything that updates

	//---------------------------------------------
	//update all NPC's in level
	//------------------------------------------

	//this is an updated method as opposed to the videos to maintain compliance with lists in c++ 11
	for( Iter = npc.begin(); Iter != npc.end();)
	{
		(*Iter)->idleUpdate();

		if((*Iter)->isAlive() == false)
		{			
			Sprite *temp = *Iter;	//get a temp reference to the Iter pointer so it can be deleted and deconstructed correctly
			delete temp;			//deletes the refernce, allowing the deconstructor to be called correctly
			Iter = npc.erase(Iter);	//delete the NPC from the list

		}
		else
		{
			//build a count of current cheese and mice for
			//HUD dislpay
			if((*Iter)->classID == MOUSE_CLASSID)		
			{
				Mouse * curMouse = static_cast<Mouse*>((*Iter));
				//check if mouse has eaten cheese
				//mice that have eaten cheese are not active
				if(!curMouse->eatenCheese())
					mice++;
			}
			if((*Iter)->classID == CHEESE_CLASSID)		
				cheese++;

			++Iter;		//iterate through the array
		}
	}

	//check the player type for correct HUD display
	//TODO
	//HUD redraws on everyframe may be slowing
	//the engine down, redraw once a second?
	//if(player->classID == MAGE_CLASSID && HUDRedraw >= 24)
	if(player->classID == MAGE_CLASSID)
	{
		Mage * magePlayer = static_cast<Mage*>(player);

		drawHud(magePlayer->getScore(), magePlayer->getLives(),  cheese, mice);
		HUDRedraw = 0;
	}

	HUDRedraw++;
	//---------------------------------------------
	//end NPC update
	//------------------------------------------
}

void Level::drawAllNpc()
{
		for( Iter = npc.begin(); Iter != npc.end();)
	{
		//use the move function to draw sprites back into same place
		(*Iter)->draw((*Iter)->getX(), (*Iter)->getY());
		++Iter;		//iterate through the array
	}
}

void Level::addEnemies(int num)
{
	int i = num;		//number of enemies to create

	while (i > 0)		//will loop until all enemies are created
	{
		int xpos = int((float(rand() % 100) / 100) * (width - 2) + 1);		//gets a random x posisiton within the level, taking into account the walls
		int ypos = int((float(rand() % 100) / 100) * (height - 2) + 1);		//gets a random y posisiton within the level, taking into account the walls

		if(gameLevel[xpos][ypos] != TILE_WALL)								//make sure current position is not a wall, no spawning in walls
		{
			Enemy *temp = new Enemy(this, levelDrawArea, SPRITE_ENEMY, (float)xpos, float(ypos));		//temp pointer to a new enemy

			temp->addGoal(player);	//set the goal to the player

			addNPC((Sprite *)temp);	//add the npc to the list, cast the temp to a Sprite, all enemies have inherited from Sprite

			i--;	//enemy has been created, de-increment the loop
		}
	}
}

void Level::addMouse(int num)
{
	int i = num;		//number of enemies to create

	while (i > 0)		//will loop until all enemies are created
	{
		int xpos = int((float(rand() % 100) / 100) * (width - 2) + 1);		//gets a random x posisiton within the level, taking into account the walls
		int ypos = int((float(rand() % 100) / 100) * (height - 2) + 1);		//gets a random y posisiton within the level, taking into account the walls

		if(gameLevel[xpos][ypos] != TILE_WALL)								//make sure current position is not a wall, no spawning in walls
		{
			Mouse *temp = new Mouse(this, levelDrawArea, SPRITE_MOUSE, (float)xpos, float(ypos));		//temp pointer to a new enemy

			temp->addGoal(player);	//set the goal to the player

			addNPC((Sprite *)temp);	//add the npc to the list, cast the temp to a Sprite, all enemies have inherited from Sprite

			i--;	//enemy has been created, de-increment the loop
		}
	}
}

void Level::addCheese(int num)
{
	int i = num;		//number of enemies to create

	while (i > 0)		//will loop until all enemies are created
	{
		int xpos = int((float(rand() % 100) / 100) * (width - 2) + 1);		//gets a random x posisiton within the level, taking into account the walls
		int ypos = int((float(rand() % 100) / 100) * (height - 2) + 1);		//gets a random y posisiton within the level, taking into account the walls

		if(gameLevel[xpos][ypos] != TILE_WALL)								//make sure current position is not a wall, no spawning in walls
		{
			Cheese *temp = new Cheese(this, levelDrawArea, SPRITE_CHEESE, (float)xpos, float(ypos));		//temp pointer to a new enemy

			//temp->addGoal(player);	//set the goal to the player

			addNPC((Sprite *)temp);	//add the npc to the list, cast the temp to a Sprite, all enemies have inherited from Sprite

			i--;	//enemy has been created, de-increment the loop
		}
	}
}

void Level::addNPC(Sprite *spr)
{
	npc.push_back(spr);
}

void Level::drawHud(int arg_score, int arg_lives, int arg_cheeseLeft, int arg_miceActive)
{
	int startLocW = levelDrawArea->getScreenWidth() + 5;
	int startLocH = levelDrawArea->getScreenHeight() - 10;
	std::string headerMessage = "------HUD------";
	int counter = 0;
	std::string toPrint;	

	for (int x = 0; x < headerMessage.length(); x++)
	{
		levelDrawArea->drawHud(headerMessage.at(x), startLocW  + counter, startLocH);

		counter ++;
	}

	counter = 0;
	startLocH++;
			
	std::stringstream ss;
	ss << "Score: " << arg_score;
	toPrint = ss.str();

	for (int x = 0; x < toPrint.length(); x++)
	{
		levelDrawArea->drawHud(toPrint.at(x), startLocW  + counter + 2, startLocH);

		counter ++;
	}

	counter = 0;
	startLocH++;
			
	ss.str(" ");
	ss << "Lives: " << arg_lives;
	toPrint = ss.str();

	for (int x = 0; x < toPrint.length(); x++)
	{
		levelDrawArea->drawHud(toPrint.at(x), startLocW  + counter + 2, startLocH);

		counter ++;
	}
	//clear any trailing char as lives count down
	levelDrawArea->drawHud(' ', startLocW + toPrint.length() + 2, startLocH);
}

int Level::countMice()
{
	int mice = 0;

	for( Iter = npc.begin(); Iter != npc.end();)
	{
		if((*Iter)->classID == MOUSE_CLASSID)		
		{
			Mouse * curMouse = static_cast<Mouse*>((*Iter));
			if(!curMouse->eatenCheese())
				mice++;
		}

		++Iter;		//iterate through the array
	}
	
	return mice;
}