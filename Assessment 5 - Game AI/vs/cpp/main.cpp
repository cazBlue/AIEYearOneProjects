#include <Windows.h>

#include "Game.h"

void main()
{

	Game* myGame = new Game("Zippy paths", 1280, 720);
	myGame->Run();
	delete myGame;

}