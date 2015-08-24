#include "Bullet.h"
#include "Game.h"

#include <iostream>

#include "TextureManager.h"

//bullet defining own texture
Bullet::Bullet(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, 
	Mat3 startLocation, Vec2 a_pivot, Actor *a_parent, Actor *a_spawningActor, float a_scale)
	: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho,  textureFileName, startLocation, a_pivot, a_parent)
{
	//base bullet id, will be overridin in enemy turret/player turret classess
	m_classID = CLASSID_A_BULLET;
	//TODO make bullet firing physically accurate, say if we had different bullet types

	//set the movement vector, ramp up the speed
	m_velocity = startLocation.getUpVec() * 1000;
	
	//get the spawning actors current location 
	(*m_node->manipMat3()) = startLocation; //- a_spawningActor->GetPivot();//a_spawningActor->getNode()->getFinalTransform();

//	(*m_node->manipMat3()).Translate(Vec2(a_spawningActor->GetPivot().x,0));
	(*m_node->manipMat3()).SetScale(1,1);

	//set the bullet scale - 1 by default (takes into account current scale)
	(*m_node->manipMat3()).SetScale(a_scale , a_scale);

	m_spawningActor = a_spawningActor;

	//setting up mass, currently at default value
	m_mass = 1;
}


//bullet using common bullet texture
Bullet::Bullet(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho ,
	Mat3 startLocation, Vec2 a_pivot, Actor *a_parent, Actor *a_spawningActor, float a_scale)
	: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho, startLocation, a_pivot, a_parent)
{
	
	m_texture = new Texture(m_mainGame->getTextMngr(), TEX_ID_BULLET);
	
	//base bullet id, will be overridin in enemy turret/player turret classess
	m_classID = CLASSID_A_BULLET;
	//TODO make bullet firing physically accurate, say if we had different bullet types

	//set the movement vector, ramp up the speed
	m_velocity = startLocation.getUpVec() * 1000;
	
	//get the spawning actors current location 
	(*m_node->manipMat3()) = startLocation; //- a_spawningActor->GetPivot();//a_spawningActor->getNode()->getFinalTransform();

//	(*m_node->manipMat3()).Translate(Vec2(a_spawningActor->GetPivot().x,0));
	(*m_node->manipMat3()).SetScale(1,1);

	//set the bullet scale - 1 by default (takes into account current scale)
	(*m_node->manipMat3()).SetScale(a_scale , a_scale);

	m_spawningActor = a_spawningActor;

	//setting up mass, currently at default value
	m_mass = 1;
}



void Bullet::update(float a_dt)
{
//	(*m_node->manipMat3()).SetScale(m_scale , m_scale);

//	Actor::update(a_dt); //if update code is added to actor

	Vec2 pos = (*m_node).getFinalTransform().GetTranslation();


	//update the bullets POS
	(*m_node->manipMat3()).Translate(m_velocity * a_dt); //*= m_node->manipMat3()->Translate(m_dir);
}

void Bullet::IsColliding(Actor* a_otherActor)
{
	//m_spawningActor->getParent()

	//check to make sure we arn't collidiing with anything related to
	//the spawning actor

	if(a_otherActor->m_classID == CLASSID_A_ACTOR_EMPTY)
	{
		int temp = 0;
	}

	
	//check if the spawning actor is still alive, if not we can;t be hitting it!
	if(m_spawningActor)
	{
		//cehck if we have a valid target
		//targets can't be the spawning object or other bullets	
		if(checkValidHit(a_otherActor, m_spawningActor))
		{
	//		std::cout << "bullet his hit something " << a_otherActor->m_classID << std::endl;

			//TODO insert damage system for players and baddies
		
			//TODO bullet should wait for call from colliding object before destroy
			//otherwise the object might try and access the bullet after it's gone
			//if(a_otherActor->m_classID == CLASSID_A_ENEMYTANK m_classID
			//remove the bullet on impact
			m_actorAlive = false;
		}
	}
	else
		m_actorAlive = false;
}

bool Bullet::checkValidHit(Actor* a_actorToCheck)
{
	//check if the spawning actor is still alive
	if(m_spawningActor)
		return checkValidHit(a_actorToCheck, m_spawningActor);
	else
		return checkValidHit(a_actorToCheck, 0);
}

bool Bullet::checkValidHit(Actor* a_actorToCheck, Actor* a_parentActor)
{
	if(a_actorToCheck->m_classID != CLASSID_A_BULLET)//ignore bullet collisions
	{
		//check if parent actor is alive, it might have died :( also check if it has a valid
		//class id, there was an error where true could be returned
		//even if the actor is dead
		if(a_parentActor->m_actorAlive && a_parentActor->m_classID)
		{
			if(a_parentActor->getParent() == 0)
			{
				if(a_actorToCheck == a_parentActor)
					return false;

				return true;				
			}
			else
			{
				//check if the actor is the same
				if(a_actorToCheck == a_parentActor)
				{
					return false;
				}
				else //if not the same run check again on the parent object
					checkValidHit(a_actorToCheck, m_spawningActor->getParent());
			}
		}
		else
			return false;
	}
	else
		return false;
	
}

void Bullet::IsOutsideScreen()
{
	//bullet is outside the screen, remove it
	//in next update
	m_actorAlive = false;

//	std::cout << "Bullet outside screen - removing" << std::endl;
}