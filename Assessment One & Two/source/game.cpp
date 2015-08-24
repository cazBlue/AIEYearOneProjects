#include "Game.h"
#include <conio.h>
#include <Windows.h>

#include <iostream>

#include <string.h>

using namespace std;

//this will give us 30FPS
//game speed is the FPS limiter
#define GAME_SPEED 33.33

enum
{
	GAMEID_EVILMONKEY,
	GAMEID_MOUSEHUNT
};




bool Game::run()
{	
	endGame = false;
	gameState = GAME_SPLASH;

	//if there is no table create one
	//if it exists this will have no affect so it's fine to call it
	gameLeaderBoard = new LeaderDB(&drawArea, Currentlevel, 26, 25);

	gameLeaderBoard->generateTable();

	//get the current leaderboard
	//gameLeaderBoard->LeaderBoardString = gameLeaderBoard->getLeaderBoard();
	
	//gameLeaderBoard->drawBoard("wh \n and then some break \n and more break \n");
	//leader lb

	gameLeaderBoard->drawBoard(gameLeaderBoard->getLeaderBoard());

	char key = ' '; //set space key
	
	startTime = timeGetTime(); // set the start time to the time of the game when it loads (this will not always be zero, say if we have a menu!
	frameCount = 0;	//counts the current frames
	lastTime = 0;	//last time a frame was updated


	// -------------------------- main in game loop
	while (key != 'q' && !endGame)
	{
		while (!getInput(&key))//stays in while loop until any key is pressed
		{
			switch(gameState)
			{
			case GAME_GAME:
				gameUpdate();
				break;
			case GAME_SPLASH:
				splashUpdate();
				break;
			case GAME_WIN:
				winUpdate();
				break;
			case GAME_LOSE:
				loseUpdate();
				break;
			case GAME_ENTERHIGHSCORE:
				addHighScoreUpdate();
				break;
			default:
				cout << "error loading game" << endl;
			}
		}
		switch(gameState)
		{
		case GAME_GAME:
			Currentlevel->keyPress(key);//passess input from the player to the current level 
			break;
		case GAME_SPLASH:
			splashKeyHandler();
			break;
		case GAME_WIN:
			winKeyHandler();
			break;
		case GAME_LOSE:
			loseKeyHandler();
			break;
		case GAME_ENTERHIGHSCORE:
			addHighScoreKeyHandler();
			break;
		default:
			Currentlevel->keyPress(key);//passess input from the player to the current level 
		}
		
	}
	//-----------------------------game loop ends-----------------------

	//cout << frameCount / ((timeGetTime() - startTime) / 1000) << " fps " << endl;
	//cout << frameCount << endl;
	//player is a pointer and must be deleted to avoid memory leaks	


	//cout << "End of game: " << key << endl;

	/*
	//TODO create game pointer list for deconstructing
	//clean game pointers
	*/
	delete gameLeaderBoard;
	
	//pointer cleaning
	
	return true;	
}

//check to see if the keyboard has been hit and updates the pointer of the passed in character if it has
bool Game::getInput(char *c)
{
	if (_kbhit())//check if the keyboard has been hit
	{
		*c = _getch(); //update reference of a char to the value of the hit key
		return true;
	}
	return false;
}

//controls the framerate for the game
void Game::timerUpdate(void)
{
	//declare the current time and take it from the last time
	double currentTime = timeGetTime() - lastTime;

	//1000 milliseconds / 30 (desired framerate) gives the value to check for frame increment

	if(currentTime < GAME_SPEED)
		return;	//if the time is less than the desired frame do not increment the frame
	
	//------------------------------------------------------
	//----------update frame draw code in here--------------
	
	Currentlevel->update();

	//----------------------end frame------------------
	//-------------------------------------------------

	//increment the frame count
	frameCount++;
	//set the last time variable for the next update
	lastTime = timeGetTime();
}

void Game::splashUpdate()
{
	//TODO - add update code
	//put loop code in after message is displayed
	drawArea.clearScreen(drawArea.getScreenWidth(), drawArea.getScreenHeight());
	
	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 6);
	cout << "++++++++++++++ MOUSE HUNTER ++++++++++++" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 5);
	cout << "++++++++++++++++++++++++++++++++++++++++++" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 4);
	cout << "Eat cheese and slay mice to win" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 3);
	cout << "touching mice and mice eating cheese hurts you" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 2);
	cout << "Mice that have eaten cheese cannot be harmed!" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 - 1);
	cout << "Move: WASD" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 23, drawArea.getScreenHeight() / 2 );
	cout << "Shoot: Spacebar" << endl;

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 21, drawArea.getScreenHeight() / 2 + 2);
	cout << "========Press Space to begin========" << endl;

	

	while (key != ' ')
	{
		while (!getInput(&key))//stays in while loop until any key is pressed
		{

		}

	}
	
	//---------exit state block
	//clear and redraw the game screen
//	drawArea.clearScreen(drawArea.getScreenWidth(), drawArea.getScreenHeight());
//	drawArea.drawBackground();
//	Currentlevel->drawAllNpc();
//	player->draw(player->getX(), player->getY());
	//----------resume the main game loop
	newGame();
	gameState = GAME_GAME;	

}

void Game::gameUpdate()
{
	timerUpdate(); //timer update limits and controls the FPS of the game	
	
	int endType;
	if(player->checkGameOver(&endType))	
	{
		if(endType == ENDTYPE_WIN)
		{
			gameState = GAME_WIN;
		}
		else
		{
			gameState = GAME_LOSE;
		}
		key = 0;
	}
}


void Game::winUpdate()
{
	//clear the screen
	drawArea.clearScreen(drawArea.getScreenWidth(), drawArea.getScreenHeight());
	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2);

	//score is pre-sorted in sql query
	//working through the array from 0 up means checking top score first
	DBLeaderBoard lb = gameLeaderBoard->getLeaderBoard();
	int pScore = player->getScore();
	
	bool addScore = false;
	int idScoreReplace;
	string playerName;

	for(int i = 0; i < 10; i++)
	{
		int dbScore = lb.score[i];
		if(pScore >= dbScore && !addScore)
		{						
			addScore = true;
			idScoreReplace = lb.id[i];//get the ID of the score to replace
			break;
		}
	}

	if(addScore)
	{		
		cout << "You got a high score of: " << pScore << endl;
		drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 1);
		cout << "Enter your name!" << endl;
		drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 2);
		cin >> playerName;
	

		if(gameLeaderBoard->updateScore(playerName, pScore, idScoreReplace))
		{
			drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 3);
			cout << "You Score is added!" << endl;						
		}
		else
		{
			drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 4);
			cout << "error adding score" << endl;		
		}
	}
	else
	{
		drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 2);
		cout << "You didn't get a high score" << endl;
		drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 3);
		cout << "Better luck next time!" << endl;
		cin.get();
	}

	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 5);
	cout << "Press space to continue" << endl;		
	//cin

	while (key != ' ')
	{
		while (!getInput(&key))//stays in while loop until any key is pressed
		{

		}
	}
	//reset key stroke for splash screen
	key = 0; 
	gameState = GAME_SPLASH;	
}

void Game::loseUpdate()
{
		//clear the screen
	drawArea.clearScreen(drawArea.getScreenWidth(), drawArea.getScreenHeight());
	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2);

	cout << "----- :( YOU LOSE :( ------ " << endl;
	
	drawArea.gotoxy(drawArea.getScreenWidth() / 2 - 15, drawArea.getScreenHeight() / 2 + 1);

	cout << "-----PRESS SPACE TO CONTINE------ " << endl;

	while (key != ' ')
	{
		while (!getInput(&key))//stays in while loop until any key is pressed
		{

		}
	}
	//reset key stroke for splash screen
	key = 0; 
	gameState = GAME_SPLASH;	
}

void Game::addHighScoreUpdate()
{
		//TODO - add update code
}

void Game::splashKeyHandler()
{
	//TODO add kay handler code
}

void Game::winKeyHandler()
{
	//TODO add kay handler code
}

void Game::loseKeyHandler()
{
	//TODO add kay handler code
}

void Game::addHighScoreKeyHandler()
{
	//TODO add kay handler code
}

void Game::newGame()
{
	//define in child class
}