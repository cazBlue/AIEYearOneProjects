#ifndef ACTOR_H
#define ACTOR_H

#include "Texture.h"
#include "Node.h"

class Mat3;
class Game;
class Vec2;
class TexMngr;
//class colDector;

//TODO 
//change enum to use bit flags
enum CLASSID
{
	//actor classes used in tiny tanks, each derived from actor
	CLASSID_A_ACTOR,
	CLASSID_A_ACTOR_EMPTY,
	CLASSID_A_TANK,
	CLASSID_A_PLAYER_TURRET,
	CLASSID_A_ENEMY_TURRET,
	CLASSID_A_ENEMYTANK,
	CLASSID_A_BULLET,
	CLASSID_A_PLAYERBULLET,
	CLASSID_A_ENEMYBULLET
};

//TODO 
//change enum to use bit flags

enum ROTATEPOINT
{
	ROTATEPOINT_TOPLEFT,
	ROTATEPOINT_TOPRIGHT,
	ROTATEPOINT_CENTRE,
	ROTATEPOINT_BOTTOMLEFT,
	ROTATEPOINT_BOTTOMRIGHT,
};


class Actor
{
public:
	/*
	------------- Parent class for all actors
	------------- All actor based classes inherit from this class
	*/

	//actors that draw to screen but use a common texure from the texture manager
	Actor(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);

	//Actor constructor for actors that draw to screen and define their own texture
	Actor(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);

	//actor constructor for non visible tracking actors, such as turrend end
	Actor(Game *a_game, Mat3 startLocation, Vec2 a_pivot, float width, float height, Actor *a_parent = 0);

	void moveActor(Vec2 Direction, float speed);
	//void rotateActor(Vec2 Direction, float speed);

	//get the texture class attatched to this carachter
	Texture* getTexture();

	virtual ~Actor();

	//update every frame
	//takes delta time
	virtual void update(float a_dt);

	//returns a pointer to this actors node
	virtual Node* getNode();

	virtual Actor* getParent();

	//returns this actors pivot
	Vec2 GetPivot();

	//get the actual draw location center of the actor
	Vec2 getActorCentre();

	//all actors must define a class ID
	CLASSID m_classID;

	friend class SpriteBatch;
	friend class Game;
	friend class State_Game;
	friend class ColDector;
	friend class Bullet;
	friend class ActorFactory;
	friend class Enemy;

protected:
	//base function to handle colliding with specific actors
	//called by collision dector
	virtual void IsColliding(Actor* a_otherActor);
	
	//what to to is the actor gets outside the level
	//called by collision dector
	virtual void IsOutsideLevel();

	//what to do if the actor gets outside the screen
	//called by collision dector
	virtual void IsOutsideScreen();

	//void linkTexture(const char *filename);

	Texture *m_texture;

	//pointer to the parent actors node, used for transforming etc
	//this is a seperate pointer to reduce function calls
	Node *m_node;

	//pointer to the parent actor
	Actor* m_parentActor;

	ROTATEPOINT m_rotatePoint;

	GLuint *m_uiProgram, *m_MatrixIDFlat;

	float *m_ortho;

	//get a pointer to the game itself
	Game *m_mainGame;

	//is actor alive flag, alive by default
	//is set to false the game loop will delete this actor!
	bool m_actorAlive;

	//custom position offset for rendering
	Vec2 m_pivot;
};



#endif