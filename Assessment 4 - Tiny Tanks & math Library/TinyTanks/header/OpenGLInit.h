#ifndef OPENGL_TINYTANK_H
#define OPENGL_TINYTANK_H
#include <glew.h>
//#include <wglew.h>
#include "glfw3.h"
#include <vector>
#include <string>
#include <fstream>
#include "Vec2.h"

class Mat3;
//class Vec2;



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

class OpenGlTinyTank
{
public:
	static GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile);
	static GLuint CreateProgram(const char *a_vertex, const char *a_frag);
	static float* getOrtho(float *a_ortho, float left, float right, float bottom, float top, float a_fNear, float a_fFar);
	static void BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float near, float far);
	static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, float xPos, float yPos, float width, float height);
	
	static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 *vert1, Mat3 *vert2, Mat3 *vert3, Mat3 *vert4);
	
	//get the 4 corners of a quad
	//optional origin to offset
	//static void OpenGlTinyTank::getCorners(Mat3 *matrix, Vec2 &vert1, Vec2 &vert2, Vec2 &vert3, Vec2 &vert4, Vec2 &origin);

	//takes mat3 as transform matrix
	//origin as top left vertex for additional offset
	//width and height for spacing vertices
	//static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 &matrix, float width, float height, const Vec2 &origin);
	static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 &matrix, float width, float height, int rotatePoint, Vec2 &customPos = Vec2(0,0));
	
	static unsigned int LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height);

private:
	static void DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Vertex *vertexData);
};



#endif OPENGL_TINYTANK_H