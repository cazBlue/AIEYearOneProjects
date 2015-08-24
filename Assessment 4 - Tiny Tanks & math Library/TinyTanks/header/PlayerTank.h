#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include "Actor.h"

class Turret;


class PlayerTank : public Actor
{
public:
	//PlayerTank();

	//using common texture
	PlayerTank(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);
	
	//using custom texture
	PlayerTank(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent = 0);

	//update with delta time
	virtual void update(float a_DT);

private:

	//the turret of the players tank
	Turret* m_turret;

	//player index
	//possibility of multiple players, track which index
	int m_playerIndex;
};



#endif