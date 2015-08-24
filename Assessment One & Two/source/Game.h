#ifndef GAME_H
#define GAME_H

#include "drawEngine.h"
#include "mage.h"
#include "level.h"
#include "mouse.h"
#include "leaderSqlite.h"

/*
--------------------------------- CONSOLE GAME CLASS LAYOUT
//outdated!

												 GAME
									
	DRAWENGINE			LEVEL					SPRITE					
												  |
									ENEMY - FIREBALL - CHARACTER   -   CHEESE
									  |					     |
									MOUSE					MAGE


--------------------------------- END CONSOLE GAME LAYOUT
*/


enum GAMESTATE
{
	GAME_SPLASH,
	GAME_GAME,
	GAME_ENTERHIGHSCORE,
	GAME_WIN,
	GAME_LOSE
};

class Game
{
public:
	//main game loop, ovveridable by child game types
	virtual bool run();

	friend class EvilMonkey;
	friend class MouseHunt;


private:
	Level *Currentlevel;
	Mage *player;
	Mouse *mouse;	
	GAMESTATE gameState;

	//current frame number of the game
	double frameCount;
	//set as the game starts
	double startTime;
	//last time the frame was updated
	double lastTime;

	//void initGameSettings(void);		//used to switch between game modes based on the current GameMode
	//void initGameMouseHunt(void);		//used to store initial settings for mouse hunt
	//void initGameEvilMonkeys(void);		//used to store initial settings for evil monkey game mode

	int gameMode;						//used to store the current game mode for reference
	virtual void newGame();

	char key;	//set space key
	bool endGame;	

	bool genDB_Data;

	//update logic for specific game states
	void splashUpdate();
	void gameUpdate();
	void winUpdate();
	void loseUpdate();
	void addHighScoreUpdate();

	//key handlers for dealing with keyboard input
	//is based on current game state
	void splashKeyHandler();	
	void winKeyHandler();
	void loseKeyHandler();
	void addHighScoreKeyHandler();
	

	LeaderDB *gameLeaderBoard;	

protected:	
	bool getInput(char *c); //get input mainly used for single checks
	void timerUpdate(void); //controls the framerate of the game, draw code is wrapped into this function

	DrawEngine drawArea;
};

#endif