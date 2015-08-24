#ifndef TURRET_H
#define TURRET_H

#include "Actor.h"

class State_Game;

class Turret : public Actor
{
public:

	Turret(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 startLocation, Vec2 a_pivot, Actor* a_turretEnd, 
		Actor *a_parent,State_Game* a_owningGame );
	~Turret();

	//update with delta time
	virtual void update(float a_DT);

	virtual void SpawnBullet();
	
	

private:
	//TODO put in different control options

	Vec2 shootPos;

	State_Game* m_owningState;

	double m_mPosX, m_mPosY;

	//matrix that holds the end of the turret
	Actor* m_turretEnd;

	float m_fireRate, m_lastFired;
};

#endif