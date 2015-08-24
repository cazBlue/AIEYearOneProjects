#include "EnemyTurrret.h"

#include "Game.h"
#include "Bullet.h"
#include "State_Game.h"
#include "TextureManager.h"

//turret using common texture
EnemyTurret::EnemyTurret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, Actor *a_parent, State_Game* a_owningGame) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_ENEMY_TURRET;
	m_owningState = a_owningGame;
	m_turretEnd = a_turretEnd;

	m_texture = new Texture(m_mainGame->getTextMngr(), TEX_ID_ENEMY_TURRET);

	//set a default max fire rate
	m_fireRate = 1;  

	m_player = m_owningState->GetPlayer();
}

//turret defining own texture
EnemyTurret::EnemyTurret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, Actor *a_parent, State_Game* a_owningGame) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho,  textureFileName, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_ENEMY_TURRET;
	m_owningState = a_owningGame;
	m_turretEnd = a_turretEnd;

	//set a default max fire rate
	m_fireRate = 1;  

	m_player = m_owningState->GetPlayer();
}

EnemyTurret::~EnemyTurret()
{
	delete m_turretEnd;
}

void EnemyTurret::update(float a_DT)
{
	//only update if the player is alive
	if(m_actorAlive)
	{
		//update the turret to point at the current player
		TrackPlayerPos();

		//get the current running time of the game
		float totalTime = glfwGetTime();

		//get a random number between 0~1
		float r = (float)((double) rand() / RAND_MAX);
		//add the chance for a further second delay
		r += rand() % 10; 

		//fire gun at a steady rate offset by the random number
		if(totalTime - m_lastFired  >= m_fireRate + r)
		{
			m_lastFired = totalTime;
			SpawnBullet();
		}
	}
}

void EnemyTurret::SpawnBullet()
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

	//set the class ID for enemy bullet
	bul->m_classID = CLASSID_A_ENEMYBULLET;

	m_owningState->addActor(bul);
}



void EnemyTurret::TrackPlayerPos()
{
	Mat3 transMat = m_node->getFinalTransform();
	Vec2 playerPos = m_player->getNode()->getFinalTransform().GetTranslation();

	Vec2 diff = Vec2( playerPos - Vec2(transMat.GetTranslation().x + (m_texture->getWidth() * 
			transMat.GetScale().x  / 2) + m_pivot.x,
			transMat.GetTranslation().y + (m_texture->getHeight() * transMat.GetScale().x / 2) + m_pivot.y)).GetNormalised();

	//get the radian
	float rot = Vec2(0,1).GetAngleBetween(diff);


	//get the parent tanks rotation and offset the turrets rotation
	float allRot = transMat.GetUpRot();
	float thisRot = m_node->getNodeTransform().GetUpRot();
	float rotOffset = allRot - thisRot;

	//set rotation to point at mouse and offset the parents rotation
	m_node->manipMat3()->SetRotation(rot - rotOffset);
	
}