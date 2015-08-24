#include "mouse.h"
#include <iostream>

#include "cheese.h"
#include "character.h"
#include "mage.h"


Mouse::Mouse(Level *lvl, DrawEngine *de, int s_index, float x, float y, int i_lives, int smellDistance) : Enemy(lvl, de, s_index, x, y, i_lives)
{
	
	classID = MOUSE_CLASSID; 	//set in Sprite.h

	hasCheese = false;
	smellsCheese = false;

	mouseSmellDistance = smellDistance;
	targetCheese = 0;
	goal = 0;

}

void Mouse::idleUpdate(void)
{
	if(!hasCheese)
		simulateAI();
	else
		reDrawMouse();//redraw the mouse! incase anything goes over the top. mouse with cheese is awesome and can't be unseen

}

void Mouse::simulateAI(void)
{
	//------------ smell for cheese

	list <Sprite *>::iterator Iter;
	//scan x posative from mouse

	if(!smellsCheese)
	{
			//mouse was moving to fast, now has random chance to now move
		int random = rand() % 100;	//who doesn't love a random number of modulo 100

		if(random >  80)
		{
			//search for cheese
			while(!move(float(rand() % 3 - 1), float(rand() % 3 - 1)))	//tries to move in any direction until succesfull, fairly verbose method
			{
				//no function needed, it all happens in the while loop itself
			}
		}

		//smell for cheese
		for(int newX = int(pos.x) - mouseSmellDistance; newX < pos.x + mouseSmellDistance; newX++)
		{	
			for(int newY = int(pos.y) - mouseSmellDistance; newY < pos.y + mouseSmellDistance; newY++)
			{

				for (Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
				{
					if ((*Iter)->classID == CHEESE_CLASSID &&  int((*Iter)->getX()) == newX && int((*Iter)->getY()) == newY)
					{
						cheesePos.x = float(newX);
						cheesePos.y = float(newY);
						smellsCheese = true;
						targetCheese = (*Iter);
					}
				}
			}
		}
	}
		
	if(smellsCheese && targetCheese->getLives() > 0)
	{
		vector direction;						//empty direction

		direction.x = cheesePos.x - pos.x;		//get the x vector of direction to the goal (player)
		direction.y = cheesePos.y - pos.y;		//get the y vector of direction to the goal (player)
	
		float mag = sqrt(direction.x * direction.x + direction.y * direction.y);	//get the length of the current pos to the goal pos as a magnitude (speed)
		
		//mag can = 0 on smaller maps when AI spawns at the end point
		if(mag != 0)
		{
			direction.x = direction.x / (mag * 5);	//temper the magnitude (speed) of the move by increasing the number. Effectively slows the enemies down.
			direction.y = direction.y / (mag * 5);	//  "      "
		}

		//first try and move in the desired direction
		//if this direction is invalid try and move in any other direction at random
		if(!move(direction.x, direction.y))
		{		
			while(!move(float(rand() % 3 - 1), float(rand() % 3 - 1)))	//tries to move in any direction until succesfull, fairly verbose method
			{
				//no function needed, it all happens in the while loop itself
			}
		}

		if( (int(pos.x) == int(cheesePos.x) || int(pos.x)  + 1 == int(cheesePos.x) || int(pos.x) - 1 == int(cheesePos.x)) && (int(pos.y) == int(cheesePos.y) || int(pos.y) + 1 == int(cheesePos.y) || int(pos.y) - 1 == int(cheesePos.y) ))			
		{								
			targetCheese->addLives(-1);	//cheese will be cleaned up in the main level npc loop
			
			//take a life from the player
			//cheese has been eaten			
			Mage * player = static_cast<Mage*>(goal);

			player->addLives(-1); //->addLives(-1);
			

			hasCheese = true;
		}
	}
	else
	{
		//the cheese has been deleted! look for another cheese
		smellsCheese = false;
	}
}

bool Mouse::eatenCheese()
{
	return hasCheese;
}

void Mouse::reDrawMouse()
{
	draw(pos.x, pos.y);
}

void Mouse::addLives(int arg_num)
{
	Sprite::addLives(arg_num);

	Mage * player = static_cast<Mage*>(goal);

	player->addScore(10); //->addLives(-1);
	
}

