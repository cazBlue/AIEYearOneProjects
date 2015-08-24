#include "drawEngine.h"
#include <windows.h>
#include <iostream>

using namespace std;

DrawEngine::DrawEngine()
{
	//screenWidth = xSize;
	//screenHeight = ySize;

	//we don't want to see the cursor during the game, turn that sucka off.
	cursorVisibilty(false);

	map = 0;
}

//destructor sets the cursor back to visible, good practice if say we want to run multiple games
DrawEngine::~DrawEngine()
{
	cursorVisibilty(true);
	
	gotoxy(0, screenHeight + 1);
}

int DrawEngine::createSprite(int index, char c)
{
	if(index >= 0 && index < 16)
	{
		spriteImage[index] = c;
		return index;
	}	
	return -1;
}

void DrawEngine::deleteSprite(int index)
{
	// not needed in this implementation	
}

void DrawEngine::eraseSprite(int posx, int posy)
{
	gotoxy(posx, posy);
	cout << ' '<< endl;
}

void DrawEngine::drawSprite(int index, int posx, int posy)
{
	// set the cursor the desired location
	gotoxy(posx, posy);
	// draw the sprite with cout, this works as we have set the cursor to the needed location
	cout << spriteImage[index] << endl;
}

void DrawEngine::drawHud(char arg_char, int arg_posX, int arg_posY)
{
	gotoxy(arg_posX, arg_posY);

	cout << arg_char << endl;
}

void DrawEngine::drawBackground(void)
{
	if (map)
	{
		for (int y = 0; y <screenHeight; y++)
		{
			gotoxy(0, y); //reset the x position at the start of each loop
			for (int x = 0; x < screenWidth; x++)
			{
				cout << tileImage[map[x][y]];
			}
		}
	}			
}

void DrawEngine::createBackgroundTile(int index, char c)
{
	if(index >= 0 && index < 16 )//ensure the tile is not outside the set sprite index
	{
		tileImage[index] = c;
	}
}

void DrawEngine::setMap(char **data)
{
	map = data;	
}


void DrawEngine::gotoxy(int x, int y)
{
	//declare a handle, holds reference to the console
	HANDLE output_handle;
	//xy pos of the cursor on the screen
	COORD pos;
	//init the x/y
	pos.X = x;
	pos.Y = y;
	//get the console
	output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//set the consoles cursor position
	SetConsoleCursorPosition( output_handle , pos);
}

void DrawEngine::cursorVisibilty(bool visibility)
{
	//handler for the console
	HANDLE output_handle;
	//console info struct
	CONSOLE_CURSOR_INFO cciInfo;

	//set the dq size to avoid SetConsoleCursorInfo from failing 
	cciInfo.dwSize = 1;
	cciInfo.bVisible = visibility;

	//get the current console handle
	output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(output_handle, &cciInfo);
}

void DrawEngine::setScreenDimensions(int arg_x, int arg_y)
{
	screenWidth = arg_x;
	screenHeight = arg_y;
}

int DrawEngine::getScreenHeight()
{
	return screenHeight;
}

int DrawEngine::getScreenWidth()
{
	return screenWidth;
}

void DrawEngine::clearScreen(int arg_x, int arg_y)
{
	for(int x = 1; x < arg_x - 1; x++)
	{
		for(int y = 1; y < arg_y - 1; y++)
		{
			eraseSprite(x, y);
		}
	}
}