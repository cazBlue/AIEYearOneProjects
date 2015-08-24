#include "CollisionDector.h"


#include "Actor.h"

#include <iostream>

#include "Game.h"

struct ActorPoints
{
	Vec2 tl, tr, bl, br;
};

ColDector::ColDector(Game* a_game)
{
	m_game = a_game;

	m_screenHeight =  m_game->getWinHeight();
	m_screenWidth =  m_game->getWinWidth();
}

ColDector::~ColDector()
{

}

void ColDector::Update(std::list <Actor *> *a_actorList)
{
	std::list <Actor *>::iterator iter;
	for(iter = a_actorList->begin(); iter != a_actorList->end(); iter++)
	{
//		std::cout << (*iter)->m_classID << std::endl;

		std::list <Actor *>::iterator iterTwo;
		for(iterTwo = a_actorList->begin(); iterTwo != a_actorList->end(); iterTwo++)
		{
			if((*iter) != (*iterTwo)) //ignore self collisions
			{

				float actorOneRad = getRadius((*iter));
				float actorTwoRad = getRadius((*iterTwo));

				if(checkCollision((*iter), (*iterTwo), actorOneRad, actorTwoRad))
				{
					//if true these two actors are colliding
					//send a message to both actors telling them they have collided							

					if((*iter)->m_classID == CLASSID_A_BULLET)
						int temp = 0;

					if((*iter)->m_classID == CLASSID_A_ACTOR_EMPTY)
						int temp = 0;

					(*iter)->IsColliding((*iterTwo));	//notify actor one
					(*iterTwo)->IsColliding((*iter));	//notify actor two
				}
			}			
		}
		//TODO add level and screen bounds checks
		if(checkInScreen((*iter)))
			(*iter)->IsOutsideScreen();

	}
}


float ColDector::getRadius(Actor* a_actor)
{
	//function based on article here
	//http://www.gamefromscratch.com/post/2012/12/12/GameDev-math-recipes-Collision-detection-using-bounding-circles.aspx

	//Note that this currently works with uniformly scaled actors only

	if(a_actor->m_classID == CLASSID_A_BULLET)
		int temp = 0;

	//gets the scale of the actor
	float actorScale = a_actor->getNode()->getFinalTransform().GetRightScale();

	float aWidth = a_actor->getTexture()->getWidth();
	float aHeight = a_actor->getTexture()->getHeight();

	float width = (aWidth * actorScale);
	float height = (aHeight * actorScale);

	//get the width and height of the actor
	float actorWidth = (width * actorScale / 2) * (width * actorScale) / 2;
	float actorHeight = (height * actorScale / 2) * (height * actorScale) / 2;

	//get the magnitude of the actor as a square root
	return sqrt(actorWidth + actorHeight);
}

bool ColDector::checkCollision(Actor* a_actor1, Actor* a_actor2, float a_actorRadius1, float a_actorRadius2)
{
	Vec2 a1Pos = getActualPos(a_actor1);
	Vec2 a2Pos = getActualPos(a_actor2);

	float distance = (a1Pos - a2Pos).Length();

	//if the distance is less that the two radius combined there must be a collision
	if(distance < a_actorRadius1 + a_actorRadius2)
		return true;	//collision detected
	else
		return false;	//no collision
}

bool ColDector::checkInScreen(Actor* a_actor)
{
	float radius = getRadius(a_actor);

	Vec2 aPos = getActualPos(a_actor);

	float actorScale = a_actor->getNode()->getFinalTransform().GetRightScale();
	float width = (a_actor->getTexture()->getWidth() * actorScale);
	float height = (a_actor->getTexture()->getHeight() * actorScale);

	if((radius * 2) + aPos.x < 0)
		return true;
	
	if(aPos.x > m_screenWidth + width)
		return true;
	
	if((radius * 2) + aPos.y < 0)
		return true;
	
	if(aPos.y > m_screenHeight + height)
		return true;
	
	return false;
}

Vec2 ColDector::getActualPos(Actor* a_actor)
{
	//function moved to the main actor as it's a common need
	return a_actor->getActorCentre();
}