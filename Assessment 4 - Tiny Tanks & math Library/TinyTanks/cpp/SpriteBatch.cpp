#include "SpriteBatch.h"
#include "Mat3.h"
#include "Vec2.h"
#include "lodepng.h"

#include "Actor.h"

#include <iostream>

#include "Game.h"

SpriteBatch::SpriteBatch(GLFWwindow *a_window)
{
	m_window = a_window;
}

void SpriteBatch::begin()
{	
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}




void SpriteBatch::DrawCircle(float cx, float cy, float r, int num_segments, unsigned int projectionLoc, float *projectionMat) 
{ 
	float theta = 2 * 3.1415926 / float(12); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r;//we start at angle = 0 
	float y = 0; 

	struct vertex
	{
		GLfloat x, y, z, a;
	};

	

	vertex *vertices = new vertex[12];

	for(int ii = 0; ii < 12; ii++) 
	{ 
		//glVertex2f(x + cx, y + cy);//output vertex 
        
		vertices[ii].x = x + cx;

		vertices[ii].y = y + cy;
		

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 


	const float vertexPositions[] =
	{
		vertices[0].x,	vertices[0].y,		1.0f,	1.0f,
		vertices[1].x,	vertices[1].y,		1.0f,	1.0f,
		vertices[2].x,	vertices[2].y,		1.0f,	1.0f,
		vertices[3].x,	vertices[3].y,		1.0f,	1.0f,
		vertices[4].x,	vertices[4].y,		1.0f,	1.0f,
		vertices[5].x,	vertices[5].y,		1.0f,	1.0f,
		vertices[6].x,	vertices[6].y,		1.0f,	1.0f,
		vertices[7].x,	vertices[7].y,		1.0f,	1.0f,
		vertices[8].x,	vertices[8].y,		1.0f,	1.0f,
		vertices[9].x,	vertices[9].y,		1.0f,	1.0f,
		vertices[10].x,	vertices[10].y,		1.0f,	1.0f,
		vertices[11].x,	vertices[11].y,		1.0f,	1.0f,
	};
	

	const float vertexColours[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
	};

	//glColor3f(1,0,0);
	//TODO move program creation to main game to avoid creating a new program every from
	//thats causing lag when debugging
	GLuint uiProgramFlat = Game::CreateProgram("./content/shaders/VertexShader.glsl", "./content/shaders/FlatFragmentShader.glsl");

	glUseProgram(uiProgramFlat);

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMat);

	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	//specify where our vertex array is, how many components each vertex has, 
	//the data type of each component and whether the data is normalised or not
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertexPositions);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vertexColours);
	
	//draw to the screen
	glDrawArrays(GL_LINE_LOOP, 0, 12);

	glUseProgram(0);
}


float SpriteBatch::getRadius(float a_width, float a_height, float a_scale)
{
	//function based on article here
	//http://www.gamefromscratch.com/post/2012/12/12/GameDev-math-recipes-Collision-detection-using-bounding-circles.aspx

	//Note that this currently works with uniformly scaled actors only


	//gets the scale of the actor
	//float actorScale = a_actor->getNode()->getFinalTransform().GetScale().x;

	//get the width and height of the actor
	float actorWidth = (a_width * a_scale / 2) * (a_width * a_scale / 2);
	float actorHeight = (a_height * a_scale / 2) * (a_height * a_scale / 2);

	//get the magnitude of the actor as a square root
	return sqrt(actorWidth + actorHeight);
}


void SpriteBatch::end()
{
	//swap front and back buffers
	glfwSwapBuffers(m_window);
		
	//poll for and process events
	glfwPollEvents();
}

void SpriteBatch::DrawSprite(Actor *a_actor)
{
	if(a_actor->m_classID == CLASSID_A_BULLET)
		int temp = 0;

	SpriteBatch::DrawSprite((*a_actor).m_texture->m_VBO, (*a_actor).m_texture->m_IBO, (unsigned int)*(a_actor->m_uiProgram), (unsigned int)(*a_actor).m_texture->m_ui_TextureId, (unsigned int)*(a_actor->m_MatrixIDFlat), (*a_actor).m_ortho, (*a_actor).m_node->getFinalTransform(), (*a_actor).m_texture->getWidth(), (*a_actor).m_texture->getHeight(), ROTATE_CENTRE, a_actor->m_pivot);
}

void SpriteBatch::DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 &matrix, float a_width, float a_height, int rotatePoint, Vec2 a_pivot)
{
	//make sure we are drawing the current width of the object multiplied against the scale
	//otherwise changing the scale of the Mat3 isn't taken into account when drawing.
	float width = a_width * matrix.GetRightScale();
	float height = a_height * matrix.GetUpScale();

	Vec2 tl = Vec2(0,				 0)		+ a_pivot;
	Vec2 tr = Vec2(width,			 0)		+ a_pivot;
	Vec2 br = Vec2(0,			height)		+ a_pivot;
	Vec2 bl = Vec2(width,		height)		+ a_pivot;

	//set the 4 vert points to correct rotation
	tl = matrix.TransformPoint(tl);
	tr = matrix.TransformPoint(tr);
	br = matrix.TransformPoint(br);
	bl = matrix.TransformPoint(bl);


	Vertex vertexData[4] = 
	{
		Vertex(tl.x, tl.y,   1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f),
		Vertex(tr.x, tr.y,   1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 0.0f),
		Vertex(bl.x, bl.y,   1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 1.0f),
		Vertex(br.x, br.y,   1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f)
	};
	

	GLubyte indices[6] = 
	{
		0, 1, 2,	// first triangle
		0, 2, 3		// second triangle
	};

	glUseProgram(programID);

	//std::cout << glGetError() << std::endl;

	// bind our texture to active texture 0...
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int textureLocation = glGetUniformLocation(programID, "myTextureSampler");
	glUniform1i(textureLocation , 1 );
	
	//send our orthographic projection info to the shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMat);
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	
	//allocate space for vertices on the graphics card
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*4, vertexData, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*6, indices, GL_STREAM_DRAW);

	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); 
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*8));
		
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
//	glDrawArrays(GL_QUADS, 0, 4);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
	

	//debug collision circles by displaying them
	//note that collision code should match this method for positions collision circles to be correct
	
	//create a posiiton around the world 0 point for the centre of the object to transform from
//	Vec2 drawPos = Vec2((width / 2) + a_pivot.x, (height / 2) + a_pivot.y);
//
//	//transform the circle point to where it needs to go
//	drawPos = matrix.TransformPoint(drawPos);
//
//	float radius = getRadius(width, height, matrix.GetRightScale());
//	//draw the circle
//	DrawCircle(drawPos.x, drawPos.y,  radius, 12, projectionLoc, projectionMat);
	
//	
}