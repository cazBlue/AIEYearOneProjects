#include "Game.h"

int main()
{
	/* ---- run Game */
	Game *pGame = new Game(640, 480, "Callan - Overload");
	pGame->RunGame();
	delete pGame;
}