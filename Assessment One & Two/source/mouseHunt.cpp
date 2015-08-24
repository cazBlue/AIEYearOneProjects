#include "mouseHunt.h"

#include <Windows.h>

bool MouseHunt::run()
{

	//create background tiles
	drawArea.createBackgroundTile(TILE_EMPTY, ' ' );
	drawArea.createBackgroundTile(TILE_WALL, 219); //uses the ascii chart 2, see msdn

	//create game sprites
	drawArea.createSprite(SPRITE_MOUSE, 235);	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_CHEESE, '$');	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_PLAYER, 1);	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_FIREBALL, '*');	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_ENEMY, '$');	//uses the ascii chart 2, see msdn


	//set the level dimensions		
	//Currentlevel = new Level(&drawArea, 50, 25);
	Currentlevel = 0;

	newGame();

	char key = ' '; //set space key	
	frameCount = 0;	//counts the current frames
	lastTime = 0;	//last time a frame was updated

		
	//--------------------------------------------------run the super game loop
	Game::run();
	//--------------------------------------------------end super game loop


	return true;
}

void MouseHunt::newGame()
{	
	bool doDraw;
	if(Currentlevel != 0)
	{
		delete Currentlevel;
		doDraw = true;
	}
	else
		doDraw = false;


	Currentlevel = new Level(&drawArea, 50, 25);

	if(doDraw)	
	{		
		Currentlevel->draw();//draw the current level
	
		player = new Mage(Currentlevel, &drawArea, 0, 1.0f, 1.0f, 5);	
		Currentlevel->addPlayer(player);//add the player to the level - make it aware of the current player	
		player->move(0,0); //set the player to it's initial position

		//add mouse and cheese
		Currentlevel->addMouse(6);
		Currentlevel->addCheese(5);	
	}

	startTime = timeGetTime(); // set the start time to the time of the game when it loads (this will not always be zero, say if we have a menu!
}