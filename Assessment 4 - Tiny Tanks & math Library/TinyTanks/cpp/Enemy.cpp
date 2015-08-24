#include "Enemy.h"

#include "Game.h"
#include <iostream>
#include <stdlib.h>
#include "Vec2.h"
#include <time.h>
#include "Bullet.h"
#include "EnemyTurrret.h"

#include "TextureManager.h"

Enemy::Enemy(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , Mat3 startLocation, Vec2 a_pivot, Actor *a_parent) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho, startLocation, a_pivot, a_parent)
{
	//move the enemy to a random spawn location
	m_classID = CLASSID_A_ENEMYTANK;	

	m_node->manipMat3()->SetTranslation(RandomLocation());

	m_moveTarget = RandomLocation();

	m_texture = new Texture(m_mainGame->getTextMngr(), TEX_ID_ENEMY_TANK);

	//set a blank rotaion
	m_goalRotation = 0;

	//unlock the rotation to ensure the enemy does it's initial rotation
	m_rotLock = false;

	//set initial
	m_moveSpeed = 3;
}

//enemy defining own texture
Enemy::Enemy(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho,  textureFileName, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_ENEMYTANK;	
	//move the enemy to a random spawn location
	m_node->manipMat3()->SetTranslation(RandomLocation());

	m_moveTarget = RandomLocation();
	
	//set a blank rotaion
	m_goalRotation = 0;

	//unlock the rotation to ensure the enemy does it's initial rotation
	m_rotLock = false;

	//set initial
	m_moveSpeed = 3;
}

Enemy::~Enemy()
{
	//Deconstructor
}

void Enemy::update(float a_DT)
{
	//call to main actor update if needed
	//Actor::update(a_DT);


	//check how close we are to the target
	if(DistanceToTarget() < 1)
	{
		//get a new location for the tank to move to
		m_moveTarget = RandomLocation();
		//reset the rot lock
		m_rotLock = false;
	}

	//check to see if we are facing the move target
	if(FaceMoveTarget(a_DT))
	{
		//we are facing the target, move forward
		MoveToTarget(a_DT);
	}
}


Vec2 Enemy::RandomLocation()
{
	//TODO check if the enemy is in an ok place to spawn

	int width = (int)m_mainGame->getWinWidth();
	int height = (int)m_mainGame->getWinHeight();


	int xPos = std::rand() % width;
//	srand (time(NULL));
	int yPos = std::rand() % height;
		
	return Vec2(xPos, yPos);
}

float Enemy::DistanceToTarget()
{
	Vec2 enemyPos = getActorCentre();
	Vec2 targetPos = m_moveTarget;

	//we only want to compare posative lengths
	return abs(enemyPos.Length() - targetPos.Length());  
}

bool Enemy::FaceMoveTarget(float a_DT)
{
	//TODO add rotation code to face target over time
	
	//check if the enemy already has a rotation target
	if(!m_rotLock)
	{
		//get rotation to move target
		m_goalRotation = TrackPos(m_moveTarget);

		m_rotLock = true;
	}
	//create a lerp function

	float nodeRot = m_node->getFinalTransform().GetUpRot();

	float compareRot = abs(nodeRot - m_goalRotation); 

	//check the length between as they are never exactly the same
	if(compareRot > .05)
	{
		m_node->manipMat3()->SetRotation(Lerp(nodeRot, m_goalRotation, 1.5, a_DT));

		return false;
	}
	else
		return true;
}

void Enemy::MoveToTarget(float a_DT)
{
//	std::cout << " moving to target! " << std::endl;

	float newPos = (m_moveSpeed * 100) * a_DT;
	
	float rotDiff = abs(TrackPos(m_moveTarget) - m_goalRotation);

	//if the target is over 90degree we have gone passed it, get another target
	if(rotDiff > 1.5)
	{
		//get a new location for the tank to move to
		m_moveTarget = RandomLocation();
		//reset the rot lock
		m_rotLock = false;
	}
	else
		(*m_node->manipMat3()) = (*m_node->manipMat3()).CreateTranslation(0, newPos) * (*m_node->manipMat3());

}


float Enemy::TrackPos(Vec2 a_posToTrack)
{
	Mat3 transMat = m_node->getFinalTransform();
	Vec2 diff = Vec2( a_posToTrack - Vec2(transMat.GetTranslation().x + (m_texture->getWidth() * 
			transMat.GetScale().x  / 2) + m_pivot.x,
			transMat.GetTranslation().y + (m_texture->getHeight() * transMat.GetScale().x / 2) + m_pivot.y)).GetNormalised();

	//return the radian angle
	return Vec2(0,1).GetAngleBetween(diff);
}

float Enemy::Lerp(float start, float end, float percent, float a_DT)
{
	return ((start + percent*(end - start) * a_DT));
}


void Enemy::IsOutsideScreen()
{
	//get a new location for the tank to move to
	m_moveTarget = RandomLocation();
	//reset the rot lock
	m_rotLock = false;
}

void Enemy::IsColliding(Actor* a_otherActor)
{
	CLASSID actorID = a_otherActor->m_classID;

	//if it's a normal bullet or a player bullet kill the enemy
	//if it's an enemy bullet ignore it
	if(actorID == CLASSID_A_BULLET || actorID == CLASSID_A_PLAYERBULLET)
	{
		//cast the bullet down to check for valid hit
		//valid hit means that this enemy isn't colliding with a bullet it made
		Bullet* myBullet = static_cast<Bullet*>(a_otherActor);

		if(myBullet->checkValidHit(this))
		{
//			std::cout << " hit valid target " << std::endl;

			
			//mark the turret and this enemy for deletion on next update
			m_turret->m_actorAlive = false;
			m_actorAlive = false;
		}
	}
}

void Enemy::SetTurret(Actor* a_turret)
{
	m_turret = a_turret;
}