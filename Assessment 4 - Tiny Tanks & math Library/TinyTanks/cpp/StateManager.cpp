#include "StateManager.h"
#include <iostream>

/*************************
		IGameState functions
**************************/
IGameState::~IGameState()
{
}

IGameState::IGameState(std::string a_name)
{
	m_name = a_name;
}

void IGameState::Update(float a_DT)
{
}

void IGameState::Draw(SpriteBatch *a_spriteBatch)
{
}

void IGameState::Init(Game *a_game)
{
	m_game = a_game;
}

/*************************
		GameStateManager functions
**************************/

GameStateManager::~GameStateManager()
{
	//deconstructor
	//remove IGameState Pointers
	std::list <IGameState *>::iterator iter;
	for(iter = m_IGameStates.begin(); iter != m_IGameStates.end(); iter++)
	{
		delete (*iter);
	}
}

GameStateManager::GameStateManager(Game* a_game)
{
	m_game = a_game;
}

GameStateManager::GameStateManager(IGameState* IGameState)
{
	addState(IGameState);
}

void GameStateManager::addState(IGameState* a_stateToAdd)
{
	//m_IGameStates.push_back(a_stateToAdd);
	m_gameStateEvents.push_back(new GameStateEvents(PUSH, a_stateToAdd));

}

void GameStateManager::UpdateGameState(float a_DT)
{
	ProcessGameStateEvents();

	m_IGameStates.back()->Update(a_DT);
}

void GameStateManager::ProcessGameStateEvents()
{
	std::list <GameStateEvents *>::iterator iter;
	for(iter = m_gameStateEvents.begin(); iter != m_gameStateEvents.end(); iter++)
	{
		//check for a push event
		if((*iter)->cmd == PUSH)
		{
			(*iter)->state->Init(m_game);
			//push the state onto the stack
			m_IGameStates.push_back((*iter)->state);
		}
//		std::cout << (*iter)->name << std::endl;
	}

	//destroy all events, avoid duplication
	m_gameStateEvents.clear();
}

void GameStateManager::Draw(SpriteBatch *a_spriteBatch)
{
	m_IGameStates.back()->Draw(a_spriteBatch);
}