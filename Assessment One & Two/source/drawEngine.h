#ifndef DRAW_ENGINE_H
#define DRAW_ENGINE_H



//custom struct to create 2d vector
//TODO re-implement custom 2d vector class
/*
struct vector
{
	float x;
	float y;
};
*/

class Game;
//controls drawing sprites to the screen
class DrawEngine
{
public:
	//default constructor - sets the screen size of the game
	DrawEngine();
	//destructor
	~DrawEngine();
	
	//create a sprite that the draw engine is aware of, must assign an index for the char array of spriteImage, index should be unique or override an exisiting sprite
	int createSprite(int index, char c);
	//delete sprite
	void deleteSprite(int index);
	
	//erase needs no index as it simply sets the console location to space
	void eraseSprite(int posx, int posy);
	//draw sprite at index to position x,y
	void drawSprite(int index, int posx, int posy);

	void drawHud(char charToDraw, int posX, int posY);

	void createBackgroundTile(int index, char c);//assign a background tile to the sprite index
	void setMap(char **);	//set the map to the map created by level.cpp
	void drawBackground(void);	//draw the backgroud map generated in level.cpp createlevel()	

	void setScreenDimensions(int x, int y);

	void clearScreen(int x, int y);

	int getScreenWidth();
	int getScreenHeight();
	//vector getScHeightWifth();

	friend class Game;

protected:
	int screenWidth, screenHeight;//define the screen width and height
	//array holds sprite images used in the game
	char spriteImage[16];

	//level map as a 2 dimensional array
	char **map;

	char tileImage[16];

private:
	//moves the cursor to the specified location
	void gotoxy(int x, int y);

	//set the cursor visibilty
	void cursorVisibilty(bool visibility);
};


#endif