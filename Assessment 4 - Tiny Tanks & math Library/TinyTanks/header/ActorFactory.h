#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <list>


enum ACTOR_FACTORY_TYPE
{
	A_FACTORY_PLAYER,
	A_FACTORY_ENEMY,
	A_FACTORY_BULLET,
	A_FACTORY_EMPTY
};

class Actor;
class Game;
class State_Game;
/*
	creates an actor of selected type and returns a pointer to that actor 

	This class handles all internal loading elements, works in conjunction with texture manager

	This class is only for spawning actors, it makes no effort to track or clean them up

	By using the actor factory their is no need to #include every actor into the game state and others

	The factory can pull actors together to create a functioning came element, such as the player and enemy

	Be careful where to call the actor factory from, don't spawn in walls etc, the actor factory makes
	no effot to check if a spawn if valid or not, it jsut spawns actors
*/

class ActorFactory
{
public:
	ActorFactory(Game* a_game, State_Game* a_state);
	~ActorFactory();

	//actor list is the list to add the spawned actor to, spawned actor is a passed empty pointer that will hold the spawned object (requries a cast to 
	//use for child class - actor to spawn is an ENUM of actor types
	virtual Actor* spawnActor(std::list <Actor *> *a_actorList, ACTOR_FACTORY_TYPE a_actorToSpawn);

private:
	Game* m_game;
	State_Game* m_owningState;

	Actor* SpawnPlayer(std::list <Actor *> *a_actorList);

	Actor* SpawnEmpty(std::list <Actor *> *a_actorList);

	Actor* SpawnEnemy(std::list <Actor *> *a_actorList);


protected:


};


#endif 