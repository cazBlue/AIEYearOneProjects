#include <Windows.h>
#include <iostream>

//#include "Point.h"
//#include "Vector.h"

//#include "Game.h"

#include "evilMonkey.h"
#include "mouseHunt.h"
//delcare that we want to use the std namespace in the current scope , no need to do std::cout
using namespace std;


//main program loop
int main()
{
	//declare our game
	EvilMonkey gameMonkey;
	MouseHunt gameMouseHunt;

	//begine the main game loop
	//gameMonkey.run();
	gameMouseHunt.run();

	//stop the console exiting instantly once the game is done
	system("Pause");
	
	return 0;
}