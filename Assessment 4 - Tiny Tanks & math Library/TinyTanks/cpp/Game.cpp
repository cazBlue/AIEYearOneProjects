#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "StateManager.h"
#include "State_Game.h"

#include "TextureManager.h"

//used for random numbers
#include <stdlib.h>
#include <time.h>


using namespace std;

Game::~Game()
{
	//TODO decontructor as needed

	delete m_spriteBatch;
	//delete m_window;
	delete m_orthoProjection;
	//delete m_name;
//	delete m_gameManager;		//TODO check this variable for memory leak
	//clear all pointers to actors out of memory
//	std::list <Actor *>::iterator iter;
//	for(iter = actors.begin(); iter != actors.end(); iter++)
//	{
//		delete (*iter);
//	}

	delete m_texManager;
}

Game::Game(float a_width, float a_height, const char *a_name)
{
	m_deltaTime = 0; m_lastTime = 0; m_currentTime = 0;

	m_width = a_width;
	m_height = a_height;
	m_name = a_name;
	m_orthoProjection = new float[16];

	/* initialize random seed: */
	srand (time(NULL));
}


void Game::LoadGame()
{
	std::cout << "Loading Game" << std::endl;

	glfwWindowHint(GLFW_VERSION_MAJOR, 4.0f);
	glfwWindowHint(GLFW_VERSION_MINOR, 5.0f);

	//Initialise GLFW
	if(!glfwInit())
	{
		//return -1;
		exit(1);
	}

	//create a windowed mode window and it's OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		exit(1);
		//return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(m_window);
	
	//============================= move the window to second monitor
	//set the windows position
	glfwSetWindowPos(m_window, 2380, 100);
	//set the consoles position
	MoveWindow(GetConsoleWindow(),1921,50,400,200,1);
	
	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		exit(1);
		//return -1;
	}

	//create shader program
	m_uiProgram = Game::CreateProgram("./content/shaders/VertexShader.glsl", "./content/shaders/TexturedFragmentShader.glsl");

	//find the position of the matrix variable in the shader so we can send info there later
	m_MatrixIDFlat = glGetUniformLocation(m_uiProgram, "MVP");

	//set up an ortho at the top left and with 100 levels of z deth
	Game::BuildOrthoProjectionMatrix(m_orthoProjection, 0, m_width, m_height, 0, 0, 100);

	//TODO could this be better as a static class?
	//create a sprite batch object 
	m_spriteBatch = new SpriteBatch(m_window);

	//set the call back to enable esc to exit program
	glfwSetKeyCallback(m_window, key_callback);

	//create the game state manager
	//the manager needs a reference to "this" for pulling information
	m_gameManager = new GameStateManager(this);

	//create the texture manager, this loads common textures
	m_texManager = new TexMngr();
	m_texManager->LoadCommonTextures();
}


void Game::RunGame()
{
	//load initial game settings
	LoadGame();


	//set the inital game statestate
	State_Game *curGameState = new State_Game("Main Game");
	m_gameManager->addState(static_cast<IGameState*>(curGameState));

	//loop until the user closes the window
	while(!glfwWindowShouldClose(m_window))
	{		
		m_lastTime		= m_currentTime;		
		m_currentTime	= glfwGetTime();
		m_deltaTime		= m_currentTime - m_lastTime;

		//show how many actors are in the game
//		std::cout << actors.size() << std::endl;


		//=====================================
		//------------------  update all actors via state manager
		//=====================================
		m_gameManager->UpdateGameState(m_deltaTime);
		//=====================================
		//------------------  draw all actors via state manager
		//=====================================		
		m_gameManager->Draw(m_spriteBatch);
	}
	//program is over, exit
	glfwTerminate();
}



GLuint Game::CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
{
	std::string strShaderCode;
	//open shader file
	std::ifstream shaderStream(a_strShaderFile);
	//if that worked ok, load file line by line
	if(shaderStream.is_open())
	{
		std::string Line = "";
		while(std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const *szShaderSourcePointer = strShaderCode.c_str();
	
	//create shader ID
	GLuint uiShader = glCreateShader(a_eShaderType);
	//load source code
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);

	//compile shader
	glCompileShader(uiShader);

	//check for compilation errors and output them
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}

GLuint Game::CreateProgram(const char *a_vertex, const char *a_frag)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint uiProgram = glCreateProgram();

	//attach shaders
	for(auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		glAttachShader(uiProgram, *shader);

	//link program
	glLinkProgram(uiProgram);

	//check for link errors and output them
	GLint status;
	glGetProgramiv (uiProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}


void Game::BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float a_near, float a_far) 
{
	float w = right - left;
	float h = top - bottom;
	float d = a_far - a_near;

	pMat4x4[0]	=  2.0f / w;
	pMat4x4[1]	=  0.0f;
	pMat4x4[2]	=  0.0f;
	pMat4x4[3]	=  0.0f;
	pMat4x4[4]	=  0.0f;
	pMat4x4[5]	=  2.0f / h;
	pMat4x4[6]	=  0.0f;
	pMat4x4[7]	=  0.0f;
	pMat4x4[8]	=  0.0f;
	pMat4x4[9]	=  0.0f;
	pMat4x4[10] =  2.0f / d;
	pMat4x4[11] =  0.0f;
	pMat4x4[12] = -1.0f * ( (right + left) / w );
	pMat4x4[13] = -1.0f * ( (top + bottom) / h );
	pMat4x4[14] = -1.0f * ( (a_far + a_near)   / d );
	pMat4x4[15] =  1.0f;
}

//----------------------   callbacks
void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "key pressed" << endl;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void Game::error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

GLFWwindow* Game::getWindow()
{
	return m_window;
}

float Game::getWinWidth()
{
	return m_width;
}

float Game::getWinHeight()
{
	return m_height;
}

GLuint* Game::getUiProgram()
{
	return &m_uiProgram;
}

GLuint* Game::getMatrix()
{
	return &m_MatrixIDFlat;
}

float* Game::getOrtho()
{
	return m_orthoProjection;
}

float Game::getRndm(unsigned int a_max)
{
	return std::rand() % a_max;
}

TexMngr* Game::getTextMngr()
{
	return m_texManager;
}