#include "Actor.h"
#include "Mat3.h"
#include "Vec2.h"
//TODO remove
#include <iostream>



//empty actor  - doesn;t draw to screen
Actor::Actor(Game *a_game, Mat3 a_startLocation, Vec2 a_pivot, float width, float height, Actor *a_parent)
{
	m_classID = CLASSID_A_ACTOR_EMPTY;

	Mat3 newStartMat = a_startLocation;
	
	Vec2 matLoc = a_startLocation.GetTranslation();

	Vec2 newStartVec = matLoc - a_pivot;

	newStartMat.SetTranslation(newStartVec);

	Mat3 startLoc = newStartMat;
	
	m_node = new Node(startLoc);

	m_mainGame = a_game;

	m_actorAlive = true;

	m_pivot = a_pivot;

	//set the parent node if one is passed
	//otherwise this is a stand alone node at creation
	if(a_parent)
		m_node->setParentNode(a_parent->m_node);
	else
		m_node->setParentNode(0);
}

//actor using texture from texture manager
Actor::Actor(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 a_startLocation, Vec2 a_pivot, Actor *a_parent)
{
	m_classID = CLASSID_A_ACTOR;

//	m_texture = new Texture(textureFileName);
	
	m_node = new Node(a_startLocation);

	m_parentActor = a_parent;

	m_MatrixIDFlat = a_MatrixIDFlat;

	m_ortho = a_ortho;

	m_uiProgram = a_program;

	m_mainGame = a_game;

	m_actorAlive = true;

	m_pivot = a_pivot;

	//set the parent node if one is passed
	//otherwise this is a stand alone node at creation
	if(a_parent)
	{
		m_node->setParentNode(a_parent->m_node);
		m_parentActor = a_parent;
	}
	else
	{
		m_node->setParentNode(0);
		a_parent = 0;
	}
}



//actor define own texture
Actor::Actor(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho,  const char *textureFileName, Mat3 a_startLocation, Vec2 a_pivot, Actor *a_parent)
{
	m_classID = CLASSID_A_ACTOR;

	m_texture = new Texture(textureFileName);
	
	m_node = new Node(a_startLocation);

	m_parentActor = a_parent;

	m_MatrixIDFlat = a_MatrixIDFlat;

	m_ortho = a_ortho;

	m_uiProgram = a_program;

	m_mainGame = a_game;

	m_actorAlive = true;

	m_pivot = a_pivot;

	//set the parent node if one is passed
	//otherwise this is a stand alone node at creation
	if(a_parent)
	{
		m_node->setParentNode(a_parent->m_node);
		m_parentActor = a_parent;
	}
	else
	{
		m_node->setParentNode(0);
		a_parent = 0;
	}
}

Actor::~Actor()
{
	//remove actor pointers
	delete m_texture;
	delete m_node;
	//delete m_classID;
}

Texture* Actor::getTexture()
{
	return m_texture;
}

void Actor::update(float a_dt)
{
	//for child classes

}

Node* Actor::getNode()
{
	return m_node;
}

Vec2 Actor::GetPivot()
{
	return m_pivot;
}


void Actor::IsColliding(Actor* a_otherActor)
{
	//each child class must define this behavior

//	if(a_otherActor->m_classID != CLASSID_A_TANK || a_otherActor->m_classID != CLASSID_A_TURRET)
	
//	std::cout << "collision with: " << a_otherActor->m_classID << std::endl;

}

void Actor::IsOutsideLevel()
{
	//each child class must define this behavior
}

void Actor::IsOutsideScreen()
{
	//each child class must define this behavior
}

Actor* Actor::getParent()
{
	return m_parentActor;
}


Vec2 Actor::getActorCentre()
{
	float actorScale = getNode()->getFinalTransform().GetRightScale();
	float width = m_texture->getWidth() * actorScale;
	float height = m_texture->getHeight() * actorScale;

	//create a posiiton around the world 0 point for the centre of the object to transform from
	//Note: remember we are storing pivots as a negative number
	Vec2 actorPos = Vec2((width / 2) + m_pivot.x, (height / 2) + m_pivot.y);

	//transform the circle point to where it needs to go
	actorPos = m_node->getFinalTransform().TransformPoint(actorPos);

	return actorPos;
}