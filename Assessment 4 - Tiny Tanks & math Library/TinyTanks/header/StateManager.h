#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Actor.h"
#include <list>
#include "SpriteBatch.h"

/*
	By design only the "back/bottom" of the stack is the current state and
	as such is the only one that is updated.

	Any other states in the stack are considered "paused" and do not update
	
	The GameStateManager class manages the list of IGameState classes.
	
	This class is heavily based on a class created by Aaron Cox for XnA, 
	I liked the structure and have made modifications in the porting process
	for how I envisiage this operating

++++++++++++++	GameStateManager commands ++++++++++++++++++++
	The game state manager makes use of a list to collect commands and process them all before
	running the update loop. This ensures commands are done in the order they where recieved.
	This type of structuring is new to me and I wanted to see how it worked.

	TODO if desired, have every game state continue to render, would allow 
	a menu to overlay the game etc
*/



class IGameState
{
public:
	~IGameState();
	IGameState(std::string a_name);
	//update the game state
	virtual void Update(float a_deltaTime);
	//draw the game state
	virtual void Draw(SpriteBatch *a_spriteBatch);
	//initial setup for each state
	virtual void Init(Game *a_game);


	//the game state manager will need access to this class
	friend class GameStateManager;
protected:
	//pointer to the active game
	Game* m_game;
	//arbitary name of the class
	std::string m_name;
};

class GameStateManager
{
public:
	virtual ~GameStateManager();
	GameStateManager(Game* a_game);
	//create a gamestate manager with default state
	GameStateManager(IGameState* a_gameState);
	
	//return a pointer to the current state
	IGameState* getActiveState();
	
	//add a state to the list, will become the active state
	void addState(IGameState* a_stateToAdd);

	//update active game state
	void UpdateGameState(float a_DT);
	//draw the active state
	virtual void Draw(SpriteBatch *a_spriteBatch);

	enum GameStateCommands
	{
		CHANGE,
		PUSH,
		POP
	};

    struct GameStateEvents
    {
        std::string name;
		IGameState *state;
        GameStateCommands cmd;
        GameStateEvents(GameStateCommands cmd, IGameState *a_state)
        {
			this->state = a_state;
			this->name = a_state->m_name;
            this->cmd = cmd;
        }
    };

private:
	Game* m_game;

	//list of all states being managed
	std::list <IGameState *> m_IGameStates;

	//list of events to be processed every update
	std::list <GameStateEvents *> m_gameStateEvents;

	void ProcessGameStateEvents();

};

#endif