#include "Agent.h"

#include "PathFinder.h"

#include <iostream>
#include <stdlib.h>     /* srand, rand */

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Agent::~Agent()
{
}

Agent::Agent(Vec2 a_pos, Vec2 a_vel, Vec2 a_heading, float a_maxVelocity, PathFinder* a_pathFinder)
{
	m_pos = a_pos;
	m_velocity = a_vel;
	m_heading = a_heading;
	m_maxVelocity = a_maxVelocity;

	m_pathFinder = a_pathFinder;

	mass = 1.0f;
	friction = 1.0f;

	m_wandCounter = 1000;
	m_wanderSwitch = 0.5f;

	m_pathNodeID = -1;

	hasReachedEnd = false;
}

void Agent::Update(float a_dt, Vec2 a_target)
{
	//target used for testing, mouse etc
	//not used in main build, shold be removed

	if(m_pathFinder->IsPathFound())
		FollowPath(a_dt);
	else
	{
		Wander(a_dt);
		//reset the path follow
		if(m_pathNodeID >= 0)
		{
			m_pathNodeID = -1;
			hasReachedEnd = false;
		}
	}
}

void Agent::Seek(float a_dt, Vec2 a_target)
{

	//====================seek the mouse
	Vec2 targetForce = (a_target - m_pos).GetNormalised() * m_maxVelocity; 
	force += targetForce;
	//====================end seek the mouse

	Vec2 fakeFriction = velocity * -friction;	

	
	ScreenCheck();

	acceleration = (force + fakeFriction) * (1.0f / mass);
	
	Vec2 test = acceleration * a_dt;

	velocity += test;

	Vec2 velocityFinal = velocity * .05f;

	//Velocity clamp

	if(abs(velocityFinal.x) > m_maxVelocity)
	{
		if(velocityFinal.x < 0)
			velocityFinal.x = m_maxVelocity * -1;
		else
			velocityFinal.x = m_maxVelocity;

		cout << "clamping x" << endl;
	}

	if(abs(velocityFinal.y) > m_maxVelocity)
	{
		if(velocityFinal.y < 0)
			velocityFinal.y = m_maxVelocity * -1;
		else
			velocityFinal.y = m_maxVelocity;

		cout << "clamping y" << endl;
	}


//	if(m_pathFinder->IsPathFound())
//	{
//			//info from http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-flee-and-arrival--gamedev-1303			
//		
//			int startID = m_pathFinder->m_Path.size() - 1;
//
//			float slowDist = 50;
//			Vec2 desiredVelocity = m_pos - m_pathFinder->m_Path[startID]->m_pos;
//			float distance = desiredVelocity.Length();
//
//
//			//slows agent for first path node
//			if(distance < slowDist)
//			{
//				//cout << "in range" << endl;
//
//				desiredVelocity = velocityFinal.GetNormalised()  * (distance / slowDist);
//
//				velocityFinal = desiredVelocity;
//			}
//	}

	m_pos += velocityFinal;

	//if(velocity.GetNormalised().x != 0 && velocity.GetNormalised().y != 0)
	m_heading = velocity.GetNormalised();
	
//	cout << "heading: " << m_heading.x << " " << m_heading.y << endl;
//	cout << "Velocity: " << velocity.x << " " << velocity.y << endl;


	// reset the force.
	force.x = 0.0f;
	force.y = 0.0f;
}

void Agent::Wander(float a_dt)
{


	//update wander target when time > wander switch. Avoids to erratic a movement and allows the agent to turn
	if(m_wandCounter > m_wanderSwitch)
	{
		//get radius to use
		float radius = 100;// Vec2::GetRadius(200);

		//refernce http://stackoverflow.com/questions/9879258/how-can-i-generate-random-points-on-a-circles-circumference-in-javascript

		//get random point on circle
		float angle = (float)( std::rand() ) / (float)(RAND_MAX);
		angle *= (float) M_PI * 2;
	
		m_wanderTarget = Vec2(cos(angle) * radius, sin(angle) * radius);

		m_wanderTarget += m_pos;

		//cout << "point on radius: " << m_wanderTarget.x << " " << m_wanderTarget.y << endl;

		float distance = 100;	

		Vec2 offset = m_heading * distance;

		m_wanderTarget += offset;

		m_wandCounter = 0;
	}

	Seek(a_dt, m_wanderTarget);

	m_wandCounter += a_dt;

}


void Agent::ScreenCheck()
{
	//TODO get agent width/height and include 
	//TODO include screen res variables

	float repulse = 5;

	if(m_pos.x < 30)
	{
		force.x = abs(force.x) * repulse;
	}

	if(m_pos.x > 1250)
	{
		force.x = abs(force.x) * -repulse;
	}
	
	if(m_pos.y < 30)
	{
		force.y = abs(force.y) * repulse;
	}

	if(m_pos.y > 690)
	{
		force.y = abs(force.y) * -repulse;
	}
}


void Agent::FollowPath(float a_dt)
{
	Vec2 target;

	if(m_pathNodeID == -1)//if -1 get the first node
	{
		m_pathNodeID = m_pathFinder->m_Path.size() - 1;

		//cout << "path found, tracking to first node" << endl;
	}
	else
	{
		//CHECK DISTANCE TO NODE, then get next node
		if((m_pos - m_pathFinder->m_Path[m_pathNodeID]->m_pos).Length() < 50) 
		{			

			if(!hasReachedEnd)
				m_pathNodeID--;
			else
				m_pathNodeID++;
				
//			cout << "id is: " << m_pathNodeID << endl;

			if(m_pathNodeID == m_pathFinder->m_Path.size() - 1)
				hasReachedEnd = false;			

			if(m_pathNodeID == 0)
			{
				hasReachedEnd = true;
			}
		}
	}


	//get the Vec2 target
	target = m_pathFinder->m_Path[m_pathNodeID]->m_pos;

	Seek(a_dt, target);
}