#ifndef ENEMY_TURRET_H
#define ENEMY_TURRET_H

#include "Actor.h"

class State_Game;

class EnemyTurret : public Actor
{
public:

	//turret using common texture
	EnemyTurret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, 
		Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, 
		Actor *a_parent,State_Game* a_owningGame );

	//turret defining own texture
	EnemyTurret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , 
		const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, 
		Actor *a_parent,State_Game* a_owningGame );
	~EnemyTurret();

	//update with delta time
	virtual void update(float a_DT);

	//spawn bullet
	virtual void SpawnBullet();

	friend class Enemy;

private:

	//rotates the turret to point at the player
	void TrackPlayerPos();

	Vec2 shootPos;

	State_Game* m_owningState;

	double m_mPosX, m_mPosY;

	float m_fireRate, m_lastFired;

	//matrix that holds the end of the turret
	Actor* m_turretEnd;	

	//hold a reference to the player
	//this is the target for the tank!
	Actor* m_player;
};

#endif