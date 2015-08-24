#ifndef GAME_H
#define GAME_H

#include <glew.h>
#include "glfw3.h"
#include <list>
#include "SpriteBatch.h"
#include "StateManager.h"

class TexMngr;

class Game
{
public:

	/*------------------------ functions */

	~Game();

	Game(float a_width, float a_height, const char *a_name);
	
	
	//run the main game loop, drives every other update/draw loop	
	void RunGame();

	//get a random number, used for
	//random seeds 
	float getRndm(unsigned int a_max);

	//glfw standard key callback
	//TODO move to input manager - not currently hooked up
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void error_callback(int error, const char* description);


	static void BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float near, float far);
	static GLuint CreateProgram(const char *a_vertex, const char *a_frag);
	static GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile);


	friend class GameStateManager;

public:
	/* ----------- getter setters  ---------------- */
	float getWinWidth();
	float getWinHeight();
	GLFWwindow* getWindow();

	GLuint* getUiProgram();
	GLuint* getMatrix();
	float* getOrtho();

	TexMngr* getTextMngr();

private:
	//hide the default constructor, info must be provided
	Game();
	
	//load the game - called from run game
	void LoadGame();

	//private member variables
	float m_deltaTime, m_lastTime, m_currentTime;

	//gflw window width and height
	float m_width, m_height;
	//gflw name
	const char *m_name;

	//opengl othro projection
	float *m_orthoProjection;

	//glfw pointer to window
	GLFWwindow *m_window;

	//pointer to texture manager
	TexMngr* m_texManager;

	//main shaders used in game
	//TODO create debug program
	GLuint m_uiProgram, m_MatrixIDFlat;

	SpriteBatch *m_spriteBatch;

	//std::list <Actor *> actors;			//list of actors to draw/update

	//manages the current state of the game
	GameStateManager* m_gameManager;
};


#endif