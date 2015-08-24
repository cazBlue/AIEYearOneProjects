#include "enemy.h"
#include "character.h"
#include <math.h>


//Character::Character(Level *level, DrawEngine *de, int s_index, float x, float y, int lives, char u, char d, char l, char r) : Sprite(level, de, s_index, x, y, lives)

Enemy::Enemy(Level *lvl, DrawEngine *de, int s_index, float x, float y, int i_lives) : Sprite(lvl, de, s_index, x, y, i_lives)
{
	goal = 0;
	classID = ENEMY_CLASSID; 	//set in Sprite.h
}

bool Enemy::move(float x, float y)
{
	int xpos = (int)(pos.x + x);
	int ypos = (int)(pos.y + y);

	if(isValidLevelMove(xpos, ypos))
	{
		
		list <Sprite *>::iterator Iter;
		//make sure we don't run into any other enemies
		for (Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
		{
			if((*Iter) !=this && (int)(*Iter)->getX() == xpos && (int)(*Iter)->getY() == ypos)	//don't check for this asset in the list
			{
				return false;
			}
		}

		//erase sprite at current location
		erase(pos.x, pos.y);

		//move the sprite to the new cords
		pos.x += x;
		pos.y += y;

		//set the facing direction
		facingDirection.x = x;
		facingDirection.y = y;
	
		//draw sprite at new location
		draw(pos.x, pos.y);
		
		if(goal != 0)	//make sure there is a goal to check for (evil monkey mode has the player as a goal
		{
			if ((int)goal->getX() == xpos && (int)goal->getY() == ypos)	//check if the enemy has hit the player, take a life if so 
				goal->addLives(-1);
		}
		return true;

	}
	return false;
}

void Enemy::idleUpdate(void)
{
	if(goal)
		simulateAI();
}

void Enemy::addGoal(Character *g)
{
	goal = g;
}

void Enemy::simulateAI(void)
{
	vector goal_pos = goal->getPosition();	//the goal is the current position of the player
	vector direction;						//empty direction

	direction.x = goal_pos.x - pos.x;		//get the x vector of direction to the goal (player)
	direction.y = goal_pos.y - pos.y;		//get the y vector of direction to the goal (player)
	
	float mag = sqrt(direction.x * direction.x + direction.y * direction.y);	//get the length of the current pos to the goal pos as a magnitude (speed)

	direction.x = direction.x / (mag * 5);	//temper the magnitude (speed) of the move by increasing the number. Effectively slows the enemies down.
	direction.y = direction.y / (mag * 5);	//  "      "


	//first try and move in the desired direction
	//if this direction is invalid try and move in any other direction at random
	if(!move(direction.x, direction.y))
	{		
		while(!move(float(rand() % 3 - 1), float(rand() % 3 - 1)))	//tries to move in any direction until succesfull, fairly verbose method
		{
			//no function needed, it all happens in the while loop itself
		}
	}
}