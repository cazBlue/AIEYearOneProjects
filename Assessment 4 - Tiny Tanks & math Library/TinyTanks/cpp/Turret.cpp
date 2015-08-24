#include "Turret.h"
#include "Game.h"
#include "Bullet.h"
#include "State_Game.h"

#include <iostream>

#include "TextureManager.h"

Turret::Turret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, Actor *a_parent, State_Game* a_owningGame) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_PLAYER_TURRET;
	m_owningState = a_owningGame;
	m_turretEnd = a_turretEnd;

	m_texture = new Texture(m_mainGame->getTextMngr(), TEX_ID_PLAYER_TURRET);

	//set the initial fire rate
	m_fireRate = .1;
}

Turret::~Turret()
{
	delete m_turretEnd;
}

void Turret::update(float a_DT)
{

//	std::cout << m_turretEnd.GetTranslation().x << " ---------- " << m_turretEnd.GetTranslation().y << std::endl;

	glfwGetCursorPos(m_mainGame->getWindow(), &m_mPosX, &m_mPosY);


	//get the normalised difference of the object (from centre) and the mouse pos
	//takes into account the scale of the matrix
	Vec2 diff = Vec2( Vec2(m_mPosX,m_mPosY) - Vec2(m_node->getFinalTransform().GetTranslation().x + (m_texture->getWidth() * 
		m_node->getFinalTransform().GetScale().x  / 2) + m_pivot.x,
		m_node->getFinalTransform().GetTranslation().y + (m_texture->getHeight() * m_node->getFinalTransform().GetScale().x / 2) + m_pivot.y)).GetNormalised();

	//get the radian
	float rot = Vec2(0,1).GetAngleBetween(diff);
	
	//get the parent tanks rotation and offset the turrets rotation
	float allRot = m_node->getFinalTransform().GetUpRot();
	float thisRot = m_node->getNodeTransform().GetUpRot();
	float rotOffset = allRot - thisRot;


	//set rotation to point at mouse and offset the parents rotation
	m_node->manipMat3()->SetRotation(rot - rotOffset);

	//m_turretEnd = m_node->getFinalTransform();

	//int button;
	if(glfwGetMouseButton(m_mainGame->getWindow(), GLFW_MOUSE_BUTTON_1))
	{
			//get the current running time of the game
		float totalTime = glfwGetTime();

		//get a random number between 0~1
		float r = (float)((double) rand() / RAND_MAX); 

		//fire gun at a steady rate offset by the random number
		if(totalTime - m_lastFired  >= m_fireRate + r)
		{
			m_lastFired = totalTime;
			SpawnBullet();
		}
	//	SpawnBullet();
	}
}

void Turret::SpawnBullet()
{
	float textureWidth = 128;
	float scale = .4;
	float assetScale = (textureWidth * scale) / 2;
	float bulletPos = assetScale + abs(m_pivot.x) / 2;
		
	//get our end matrix's current transform
	Mat3 spawnPos = m_turretEnd->getNode()->getFinalTransform() * m_node->getFinalTransform();

	//																										pivot offset - pivot of spawning object
	Bullet *bul = new Bullet(m_mainGame, m_uiProgram, m_MatrixIDFlat, m_ortho , spawnPos , Vec2(-assetScale,0), 0,  this, scale);
	//bul->getNode()->manipMat3()->SetScale(scale,scale);

	//set the bullet class id for player
	bul->m_classID = CLASSID_A_PLAYERBULLET;

	m_owningState->addActor(bul);
}