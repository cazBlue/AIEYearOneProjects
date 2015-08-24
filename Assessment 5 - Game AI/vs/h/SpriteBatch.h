#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <glew.h>
#include "glfw3.h"

#include "Graph.h"

 #include <ft2build.h>
 #include FT_FREETYPE_H

class Game;
class Actor;
//class Graph::Node;
class Vec2;
class Edge;
class Mat3;

class Agent;
//simple placeholder struct
//TODO replace with math library
//struct Vec2
//{
//	Vec2(float a_x, float a_y);
//	float x,y;
//};

enum SPRITE_COLOUR
{
	SPRITE_COLOUR_WHITE,
	SPRITE_COLOUR_RED,
	SPRITE_COLOUR_GREEN,
	SPRITE_COLOUR_BLUE
};


struct Vertex
{
	Vertex(float xPos, float yPos, float r, float g, float b, float a, float u, float v){
		fPositions[0] = xPos;
		fPositions[1] = yPos;
		fPositions[2] = 0;
		fPositions[3] = 1;
		fColours[0] = r;
		fColours[1] = g;
		fColours[2] = b;
		fColours[3] = a;
		fUVs[0] = u;
		fUVs[1] = v;
	}
	float fPositions[4];
	float fColours[4];
	float fUVs[2];
};


class SpriteBatch
{
public:
	///constructor, takes in main game window pointer
	SpriteBatch(Game* a_game);

	///sets up initial openGL functions
	void Init();

	//runs intial opengl frame setup
	//such as clearing the current frame/buffer
	void Begin();

	//draw actor
//	static void Draw(Actor* a_actor);
	//draw node
	void Draw(Graph::Node* a_Node, SPRITE_COLOUR a_col = SPRITE_COLOUR_WHITE);
	//draw line with optional width. will use texture width by default
	void DrawLine(Vec2 a_start, Vec2 a_end, SPRITE_COLOUR a_col = SPRITE_COLOUR_WHITE, float a_width=0);

	void DrawAgent(Agent* a_agent);

	void DrawCircle(float cx, float cy, float r);

	void DrawText(const char *text, float x, float y, float a_scale);

	//runs frame cleanup code and swaps the backbuffer to front
	void End();

	///main game window
	GLFWwindow* m_window;	

	unsigned int LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height);

private:

	static float angleBetween(Vec2 a_start, Vec2 a_end);

	static void BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float near, float far);
	static GLuint CreateProgram(const char *a_vertex, const char *a_frag);
	static GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile);

	//glfw standard key callback
	//TODO move to input manager - not currently hooked up
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void error_callback(int error, const char* description);

	//handles open portion of drawing sprite, all points must be in final position
	//final point of call for sprite, adds sprites to back buffer
	void processSprite(Vec2* a_tl, Vec2* a_tr, Vec2* a_bl, Vec2* a_br,
		GLuint a_IBO, GLuint a_VBO, unsigned int a_texID, SPRITE_COLOUR a_col);

	Game* m_game;

	//program id, projection matrix
	GLuint m_uiProgram, m_MatrixIDFlat, m_MatrixIDFlat_TextOnly; 

	float* m_orthoProjection;

	int m_width, m_height;


	//***********************
			//temp placeholder for loading textures
	
			//TODO - move to texture manager
	//*********************





	//temp placeholder to hold line, node texture
	unsigned int m_texID_line, m_line_width, m_line_height, 
				 m_texID_node, m_node_height, m_node_width,
				 m_texID_agent, m_agentWidth, m_agentHeight;

	//node IBO/VBO
	GLuint m_nodeIBO, m_nodeVBO;

	//line IBO/VBO
	GLuint m_lineIBO, m_lineVBO;

	//agent IBO/VBO
	GLuint m_agentIBO, m_agentVBO;

	//*************************************

	//handles pulling fonts from file
	FT_Library m_ft;
	//current face to display
	FT_Face m_face;
	//current glyph to display
	FT_GlyphSlot m_glyph;

	//Text shaders and objects
	GLuint m_uiTexProgram, m_Text_textureID;
	GLuint m_tex_VBO;

};




#endif