#include "evilMonkey.h"

#include <Windows.h>

//bool EvilMonkey::run(int GAMEID)
//class Game;

bool EvilMonkey::run()
{

	//create background tiles
	drawArea.createBackgroundTile(TILE_EMPTY, ' ' );
	drawArea.createBackgroundTile(TILE_WALL, 219); //uses the ascii chart 2, see msdn

	//create game sprites
	drawArea.createSprite(SPRITE_MOUSE, 235);	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_CHEESE, 234);	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_PLAYER, 1);	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_FIREBALL, '*');	//uses the ascii chart 2, see msdn
	drawArea.createSprite(SPRITE_ENEMY, '$');	//uses the ascii chart 2, see msdn


		//set the level dimensions		
	Currentlevel = new Level(&drawArea, 50, 25);
	
	player = new Mage(Currentlevel, &drawArea, 0);	

	//draw the current level
	Currentlevel->draw();

	Currentlevel->addPlayer(player);//add the player to the level - make it aware of the current player	
	player->move(0,0); //set the player to it's initial position

	Currentlevel->addEnemies(3);

	
	//--------------------------------------------------run the super game loop
	Game::run();
	//--------------------------------------------------end super game loop
	
	delete player;

	return true;
}