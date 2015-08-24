#ifndef COLLISION_DECTOR_H
#define COLLISION_DECTOR_H

#include <list>

class Actor;
class Game;
class Vec2;

struct ActorPoints;

class ColDector
{
/*
	The collision dector should be called once at the end of every update after the draw call
	The collision simply loops through every actor and notifies them if a collision has occured
	it's up to the actor whether it does anything with the collision

	Currently we are using simple circle collision detection
*/
public:

	ColDector(Game* a_game);
	~ColDector();
	/*
		update will cycle all active game objects and report any colisions back to that object as it finds them.
		note that level actors will be ignored by this check, it's up to each class to call "checkValidMove"
		before attempting to move
	*/
	void Update(std::list <Actor *> *a_actorList);

	//function that can be called by actors to see if
	//a move would result in a collision with the level itself (walls etc)
	bool CheckValidMove();

private:

	Game* m_game;

	int m_screenWidth, m_screenHeight;

	float getRadius(Actor* a_actor);

	bool checkCollision(Actor* a_Actor1, Actor* a_Actor2, float a_actorRadius1, float a_actorRadius2);

	//check if the actor is within the screen bounds
	bool checkInScreen(Actor* a_Actor);

	Vec2 getActualPos(Actor* a_actor);

};



#endif