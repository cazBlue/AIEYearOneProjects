#include "ActorFactory.h"

//os inccludes
#include <iostream>

//general includes
#include "Game.h"

//actor includes
#include "PlayerTank.h"
#include "Turret.h"
#include "EnemyTurrret.h"
#include "State_Game.h"
#include "Actor.h"
#include "Enemy.h"

ActorFactory::ActorFactory(Game* a_game, State_Game* a_state)
{
	m_game = a_game;
	m_owningState = a_state;
}

ActorFactory::~ActorFactory()
{

}

Actor* ActorFactory::spawnActor(std::list <Actor *> *a_actorList, ACTOR_FACTORY_TYPE a_actorToSpawn)
{
	
	switch (a_actorToSpawn)
	{
	case A_FACTORY_PLAYER:
		{	
			//returns the Tank actor, tank actor holds ptr to turret
			//turret holds pointer to tank end 
			return SpawnPlayer(a_actorList);				
		}
		break;
	case A_FACTORY_ENEMY:
		{
			return SpawnEnemy(a_actorList);
		}
		break;
	case A_FACTORY_BULLET:
		break;
	case A_FACTORY_EMPTY:
		{
			return SpawnEmpty(a_actorList);
		}
		break;
	default:
		std::cout << "ERROR actor not found" << std::endl;
		return 0;
		break;
	}
	
	return 0;
}

Actor* ActorFactory::SpawnEnemy(std::list <Actor *> *a_actorList)
{
	//TODO spawn enemy!

	float tankScale = .6;

	Enemy *myEnemy = new Enemy(m_game, m_game->getUiProgram(), m_game->getMatrix(), 
		m_game->getOrtho(), Mat3::CreateTranslation(23.04,23.04), 
		Vec2(-38.4,-38.4));

	//turret end does not get drawn up updated independetly, is controlled by turret object
	Actor *TurretEnd = new Actor(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho() , "CHECKER", Mat3::CreateTranslation(0,38.4), Vec2(-38.4,0), 0);

	EnemyTurret *myTurret = new EnemyTurret(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho(), Mat3::CreateTranslation(Vec2(0,0)),
		Vec2(-38.4,-38.4), TurretEnd , myEnemy, m_owningState);

	//set the enemies turret so they know about each other
	myEnemy->SetTurret(myTurret);

	a_actorList->push_back(myEnemy);
	a_actorList->push_back(myTurret);

	myEnemy->getNode()->manipMat3()->SetScale(tankScale , tankScale);

	return myEnemy;
}



Actor* ActorFactory::SpawnEmpty(std::list <Actor *> *a_actorList)
{
	Actor *emptyActor = new Actor(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho() , "CHECKER", Mat3::CreateTranslation(200,200), Vec2(0,0), 0);
	emptyActor->m_classID = CLASSID_A_ACTOR_EMPTY;
	a_actorList->push_back(emptyActor);

	return emptyActor;
}

Actor* ActorFactory::SpawnPlayer(std::list <Actor *> *a_actorList)
{
	//TODO  - create player index increment

	//set the scale for tank>turret
	float tankScale = .6;	

	//pointers are cleaned up by the game deconstructor as it pulls apart the list
	PlayerTank *myTank = new PlayerTank(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho(), Mat3::CreateTranslation(23.04,23.04), Vec2(-38.4,-38.4));
	
	//turret end does not get drawn up updated independetly, is controlled by turret object
	Actor *TurretEnd = new Actor(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho() , "CHECKER", Mat3::CreateTranslation(0,38.4), Vec2(-38.4,0), 0);

	Turret *myTurret = new Turret(m_game, m_game->getUiProgram(), m_game->getMatrix(), m_game->getOrtho(), Mat3::CreateTranslation(Vec2(0,0)),
		Vec2(-38.4,-38.4), TurretEnd , myTank, m_owningState);
	
	//add actors to update list
	a_actorList->push_back(myTank);
	a_actorList->push_back(myTurret);

	myTank->getNode()->manipMat3()->SetScale(tankScale , tankScale);

	//set the rotation to not be straight up
	myTank->getNode()->manipMat3()->SetRotation(-2.35619449);
	//get the tanks centre point for positioning on the screen
	float tankOffsetX = myTank->getTexture()->getWidth() * tankScale / 2;
	float tankOffsetY = myTank->getTexture()->getHeight() * tankScale / 2;
	//move to centre of the screen
	myTank->getNode()->manipMat3()->SetTranslation(m_game->getWinWidth() / 2 - tankOffsetX, m_game->getWinHeight() / 2 - tankOffsetY);

	return myTank;
}