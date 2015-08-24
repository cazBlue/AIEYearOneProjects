#ifndef BULLET_H
#define BULLET_H

#include "Actor.h"
#include "Turret.h"


class Bullet : public Actor
{
public:
	Bullet(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName,
		Mat3 startLocation, Vec2 a_pivot, Actor *a_parent, Actor *spawningActor, float a_scale = 1);

	Bullet(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho,
		Mat3 startLocation, Vec2 a_pivot, Actor *a_parent, Actor *spawningActor, float a_scale = 1);

	virtual void update(float a_dt);
	
	//overload for calling from other actors
	//we always want to compare against the spawning actor
	bool checkValidHit(Actor* a_actorToCheck);

private:

	//called by collision dector
	virtual void IsColliding(Actor* a_otherActor);

	//inherited check if outside screen
	void IsOutsideScreen();

	//recusively check to ensure this bullet is not colliding with any
	//part of the spawning actor, checks each parent of the spawning actors
	bool checkValidHit(Actor* a_actorToCheck, Actor* a_parentActor);


	//linear velocity of object
	Vec2 m_velocity;

	//posistion of bullet
	//get the position from the pos of the spawning object
	//Vec2 m_pos;		//not currently in use, pos is coming from actor matrix
	
	//linear force
	Vec2 m_force;

	float m_speed;
	float m_mass;

	//fake friction
	float m_damping;

	//check if bullet has left the map, mark for delete if it has
	bool checkBounds();

	Actor* m_spawningActor;	//used to ensure this bullet doesn't kill the spawning actor
};



#endif