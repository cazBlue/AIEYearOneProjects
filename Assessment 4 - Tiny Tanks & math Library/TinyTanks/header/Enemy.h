#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Vec2;

class EnemyTurret;

/*
	Base class for all enemies
	constructs a default tank that alway points at the player

	moves at random intervals

*/

class Enemy : public Actor
{
public:
	//enemy defining own texture
	Enemy(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);

	//enemy using common texture
	Enemy(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);

	~Enemy();

	//updates with delta time
	virtual void update(float a_DT);
	
	//set pointer to the enemies turret
	void SetTurret(Actor* a_turret);

private:

protected:

	Actor* m_turret;

	//picks a random location on the map
	//used for picking a place to move to
	Vec2 picRandLoc();

	//the enemy tank can only move forward and backward
	//it must rotate to face the move target
	//if not facing the target rotate a little each frame till we are facing it
	bool FaceMoveTarget(float a_DT);

	//move to target
	//not that this makes no effort to check if anything is blocking it
	//assumes a correct rotation and simply moves forward
	void MoveToTarget(float a_DT);

	//get the rotation to the target
	float TrackPos(Vec2 a_posToTrack);
	
	//lerp function, returns the current lerped amount
	//based on functionality from here, takes delta time into account
	//http://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
	float Lerp(float start, float end, float percent, float a_DT);

	//should check spawn location to ensure no
	Vec2 RandomLocation();

	//current move target for the enemy
	//Note: movement is done from the enemy centre
	Vec2 m_moveTarget;

	//set the goal rotation of the enemy
	float m_goalRotation;

	//rotation lock to avoid constant turning
	bool m_rotLock;

	//returns the distance to the enemies target
	float DistanceToTarget();

	//flag for whether we have reached the target or not
	//if not keep rotating/moving.
	//could be used for having the tank sit at an enf location for a time
	//not currently in use
	//bool m_hasReachedTarget;

	//moved speed for the 
	float m_moveSpeed;

	//if the enemy leaves the screen pick a new target
	void IsOutsideScreen();

	//handle collisions
	//currently only checks for bullets
	void IsColliding(Actor* a_otherActor);
};


#endif
