#ifndef STATE_GAME_H
#define STATE_GAME_H

#include "StateManager.h"

/*
	main game state
	controls players, enemies etc

*/

class ActorFactory;
class ColDector;
class PlayerTank;

class State_Game : public IGameState
{
public:
	~State_Game();
	State_Game(std::string a_name);
	void Init(Game *a_game);
	void Update(float a_deltaTime);
	void addActor(Actor* a_actor);
	void Draw(SpriteBatch* a_SB);
	
	//Get the current player
	//TODO enable getting of player by index
	Actor* GetPlayer();

private:
	std::list <Actor *> m_actors;			//list of actors to draw/update

	std::list <Actor *> m_debug;			//list of debug ojbects to draw/update

	ActorFactory* m_actorFactory;		//pointer to an actor factory

	ColDector* m_colDector;
	
	//TODO enable player list for multiplayer, 
	//for multi should maintain a list of players and pick one at random
	void SetCurrentPlayer(Actor* a_player);
	

	Actor* m_player;
};

#endif