#ifndef Mage_H
#define MAGE_H

#include "character.h"
#include "drawEngine.h"

enum
{
	POWERUP_CHEESE,
	POWERUP_SPEED,		//not in use
	POWERUP_FIRESPEED	//not in use

};

enum
{
	ENDTYPE_WIN,
	ENDTYPE_LOSE
};


class Mage : public Character
{
public:
	Mage(Level *level, DrawEngine *de, int s_index, float x = 1, float y = 1, int lives = 3, char spell_key = ' ',  char up_key = 'w', char down_key = 's', char left_key = 'a', char right_key = 'd');

	bool keyPress(char c);

	virtual void addLives(int num = 1); //inherited from character

	virtual bool move(float x, float y);	//inherited from Sprite

	int getScore();
	void addScore(int s);

	//TODO
	//create powerups
	bool addPowerUp(int POWERUP_TYPE);	

	//ENDTYPE enum
	bool checkGameOver(int *endType);

protected:

	void castSpell(void);
	int getMouseCount();

private:
	char spellKey;

	int score;

	bool checkForCheese();	
};

#endif