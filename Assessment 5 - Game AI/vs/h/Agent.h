#ifndef AGENT_H
#define AGENT_H

#include "Vec2.h"

class PathFinder;

class Agent
{
public:
	Vec2 m_pos;
	Vec2 m_velocity;
	Vec2 m_heading;
	
	Vec2 m_currentVelocity;

	float m_maxVelocity;


	float mass;
	float friction;
	Vec2 force;
//	Vec2 position;	
	Vec2 acceleration;
	Vec2 velocity;

	void Update(float a_dt, Vec2 a_target);

	Agent(Vec2 a_pos, Vec2 a_vel, Vec2 a_heading, float a_maxVelocity, PathFinder* a_pathFinder);
	~Agent();



	Vec2 m_wanderTarget;


private:

	void Seek(float a_dt, Vec2 a_target);	

	void Wander(float a_dt);

	void ScreenCheck();

	float m_wandCounter, m_wanderSwitch;

	void FollowPath(float a_dt);

	//id of the current node to track
	int m_pathNodeID;
	
	bool hasReachedEnd;

	//reference to pathfinder
	//used to check if a path is avialable and get the path
	PathFinder* m_pathFinder;

};


#endif AGENT_H