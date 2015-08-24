#include "fireball.h"
#include "mouse.h"

Fireball::Fireball(Level *lev, DrawEngine *de, int s_index, float x, float y, float xDir, float yDir, int i_lives) : Sprite(lev, de, s_index, x, y, i_lives)
{	
	facingDirection.x = xDir;		//set the facing direction of the sprite to the direction of the player
	facingDirection.y = yDir;

	classID = FIREBALL_CLASSID;		//set in Sprite.h
}


void Fireball::idleUpdate(void)
{
	if(Sprite::move(facingDirection.x, facingDirection.y))			//using scope operator to define the use of the move function from the Sprite class only (not use any inherited function)
	{
		list <Sprite *>::iterator Iter;

		for (Iter = level->npc.begin(); Iter != level->npc.end(); Iter++)
		{	//class id of NPC != this class id & this pos == the pos of the NPC then kill it	 -- don't kill cheese though!		
			if((*Iter)->classID != classID && (int)(*Iter)->getX() == (int)pos.x && (int)(*Iter)->getY() == (int)pos.y && (*Iter)->classID != CHEESE_CLASSID)	//check to ensure we don't worry about collidiing with other fireballs - fireballs have the same classid - don't kill cheese
			{
				//check is we are dealing with a mouse to avoid static cast errors!
				if((*Iter)->classID == MOUSE_CLASSID)
				{			
					Mouse* hitMouse =  static_cast<Mouse*>((*Iter));		//use a static class to access the moouse class - downcast the sprite to mouse

					//don't kill a mouse if he has cheese, that'd be mean
					if(!hitMouse->eatenCheese())
					{
						 (*Iter)->addLives(-1);	//take a life from the hit object
						addLives(-1);			//take a life from THIS fireball destroying it						
					}
					else
					{
						addLives(-1);			//take a life from THIS fireball destroying it												
					}
				}
				else
				{
				//it's not a mouse, kill the NPC
 				(*Iter)->addLives(-1);	//take a life from the hit object
				addLives(-1);			//take a life from THIS fireball destroying it
				}				
			}
		}
	}
	else
 		addLives(-1);	//if we hit a wall destroy the fireball
}