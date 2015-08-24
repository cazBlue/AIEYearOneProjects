#include "State_Game.h"
#include <iostream>

#include "Game.h"
#include "PlayerTank.h"
#include "Turret.h"
#include "Bullet.h"

#include "ActorFactory.h"

#include "CollisionDector.h"



State_Game::~State_Game()
{
	std::list <Actor *>::iterator iter;
	for(iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		delete (*iter);
	}

	delete m_actorFactory;
	delete m_colDector;
}

State_Game::State_Game(std::string a_name) : IGameState(a_name)
{
	//let the parent handle name assignment
	IGameState::IGameState(a_name);
}

void State_Game::Init(Game *a_game)
{
	//run parent init
	IGameState::Init(a_game);
	//set pointer to game
	m_game = a_game;

	//create an actor factory for use
	m_actorFactory = new ActorFactory(m_game, this);


	m_colDector = new ColDector(m_game);

	//spawns a default player into the centre of the world
	//player has all bits working and is ready to go.
	//also sets the current player
	//SetCurrentPlayer(static_cast<PlayerTank*>(m_actorFactory->spawnActor(&m_actors, A_FACTORY_PLAYER)));
	SetCurrentPlayer(m_actorFactory->spawnActor(&m_actors, A_FACTORY_PLAYER));
	

	//empty actor - testing only
//	m_actorFactory->spawnActor(&m_actors, A_FACTORY_EMPTY);
	
	//create 10 enemies
	for(int i = 0; i < 10; i++)
	{
		m_actorFactory->spawnActor(&m_actors, A_FACTORY_ENEMY);
	}

	std::cout << "Initalising main game state" << std::endl;
}

void State_Game::Update(float a_DT)
{
	//print out how many actors in the game
//	std::cout << m_actors.size() << std::endl;

	//update all actors
	std::list <Actor *>::iterator iter;
	for(iter = m_actors.begin(); iter != m_actors.end();)
	{
		//update actor
		(*iter)->update(a_DT);

		//check if actor is alive
		//remove is not alive
		if(!(*iter)->m_actorAlive)
		{
			//Actor *temp = ;
			delete *iter;		
			iter = m_actors.erase(iter);
		}
		else
			iter++;
	}
}

void State_Game::Draw(SpriteBatch* a_SB)
{

	a_SB->begin();

	std::list <Actor *>::iterator iter;
	for(iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		if((*iter)->m_classID == CLASSID_A_PLAYERBULLET)
		{
			int temp = 0;
		}
		a_SB->DrawSprite(*iter);

		//a_SB->DrawCircle();
	}
	a_SB->end();

	m_colDector->Update(&m_actors);
}

void State_Game::addActor(Actor* a_actor)
{
	m_actors.push_back(a_actor);
}


void State_Game::SetCurrentPlayer(Actor* a_player)
{
	m_player = a_player;
}


Actor* State_Game::GetPlayer()
{
	return m_player;
}