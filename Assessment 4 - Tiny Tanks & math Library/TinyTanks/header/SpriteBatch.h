#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <glew.h>
#include "glfw3.h"
#include <vector>
#include <string>
#include <fstream>
#include "Vec2.h"

class Actor;
class Mat3;

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


enum
{
	ROTATE_TOPLEFT,
	ROTATE_TOPRIGHT,
	ROTATE_BOTTOMLEFT,
	ROTATE_BOTTOMRIGHT,
	ROTATE_CENTRE,
	ROTATE_CUSTOM
};

class SpriteBatch
{
public:
	SpriteBatch(GLFWwindow *a_window);

	//runs intial opengl frame setup
	//such as clearing the current frame/buffer
	void begin();

	static void DrawSprite(Actor *a_actor);

	//runs frame cleanup code and swaps the backbuffer to front
	void end();

	//used for debugging
	static void DrawCircle(float cx, float cy, float r, int num_segments, unsigned int projectionLoc, float *projectionMat);

private:
	//hide default constructor
	SpriteBatch();
	
	//takes mat3 as transform matrix for the top left vert
	//width and height for spacing vertices using transform point
	static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 &matrix, float width, float height, int rotatePoint, Vec2 a_pivot);

	GLFWwindow *m_window;


	
	static float getRadius(float a_width, float a_height, float a_scale);

};


#endif