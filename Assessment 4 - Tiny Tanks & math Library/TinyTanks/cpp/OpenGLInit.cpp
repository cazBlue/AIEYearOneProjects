#include "OpenGLInit.h"
#include "Mat3.h"
#include "Vec2.h"
#include "lodepng.h"






void OpenGlTinyTank::BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float near, float far) 
{
	float w = right - left;
	float h = top - bottom;
	float d = far - near;

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
	pMat4x4[14] = -1.0f * ( (far + near)   / d );
	pMat4x4[15] =  1.0f;
}

void OpenGlTinyTank::DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 &matrix, float width, float height, int rotatePoint, Vec2 &customPos)
{
	Vec2 origin = Vec2(width / 2, height / 2);

	//switch(rotatePoint)
	//{
	//case ROTATE_TOPLEFT:
	//	break;
	//case ROTATE_CENTRE:
	//	origin = Vec2(width / 2, height / 2);
	//	break;
	//case ROTATE_BOTTOMLEFT:
	//	origin = Vec2(width, height);
	//	break;
	//default:
	//	origin = Vec2(width / 2, height / 2);
	//	break;
	//}

	Vec2 tl = Vec2(0, 0) - origin;
	Vec2 tr = Vec2(width, 0) - origin;
	Vec2 br = Vec2(0, height) - origin;
	Vec2 bl = Vec2(width, height) - origin;
	

	tl = matrix.TransformPoint(tl);
	tr = matrix.TransformPoint(tr);
	br = matrix.TransformPoint(br);
	bl = matrix.TransformPoint(bl);

	tl += width / 2;
	tr += width / 2;
	br += height / 2;
	bl += height / 2;

	Vertex vertexData[4] = 
	{
		Vertex(tl.x, tl.y,   1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f),
		Vertex(tr.x, tr.y,   1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 0.0f),
		Vertex(bl.x, bl.y,   1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 1.0f),
		Vertex(br.x, br.y,   1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f)
	};
	
	OpenGlTinyTank::DrawSprite(VBO, IBO, programID, textureID, projectionLoc, projectionMat, vertexData);
}



void OpenGlTinyTank::DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Mat3 *vert1, Mat3 *vert2, Mat3 *vert3, Mat3 *vert4)
{	
	Vertex vertexData[4] = 
	{
		Vertex(vert1->GetTranslation().x,			vert1->GetTranslation().y,			1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f),
		Vertex(vert2->GetTranslation().x,			vert2->GetTranslation().y,			1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 0.0f),
		Vertex(vert4->GetTranslation().x,			vert4->GetTranslation().y,			1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 1.0f),
		Vertex(vert3->GetTranslation().x,			vert3->GetTranslation().y,			1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f)
	};

	OpenGlTinyTank::DrawSprite(VBO, IBO, programID, textureID, projectionLoc, projectionMat, vertexData);
	//OpenGlTinyTank::DrawSprite(VBO, IBO, programID, textureID , projectionLoc, projectionMat, 10.0f, 10.0f, 50.0f, 50.0f);
}


void OpenGlTinyTank::DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, float xPos, float yPos, float width, float height)
{
	Vertex vertexData[4] = 
	{
		Vertex(xPos,			yPos,			1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f),
		Vertex(xPos + width,	yPos,			1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 0.0f),
		Vertex(xPos + width,	yPos + height,	1.0f, 1.0f, 1.0f, 1.0f,		 1.0f, 1.0f),
		Vertex(xPos,			yPos + height,	1.0f, 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f)
	};
	

	DrawSprite(VBO,IBO,programID,textureID,projectionLoc,projectionMat, vertexData);
}

void OpenGlTinyTank::DrawSprite(unsigned int VBO, unsigned int IBO, unsigned int programID, unsigned int textureID , unsigned int projectionLoc, float *projectionMat, Vertex *vertexData)
{
	GLubyte indices[6] = 
	{
		0, 1, 2,	// first triangle
		0, 2, 3		// second triangle
	};

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int textureLocation = glGetUniformLocation(programID, "myTextureSampler");
	glUniform1i(textureLocation , 1 );
	
	//send our orthographic projection info to the shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMat);
	
	//printf("PP Draw 1\n");
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//printf("PP Draw 2\n");

	//allocate space for vertices on the graphics card
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*4, vertexData, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*6, indices, GL_STREAM_DRAW);

	// bind our texture to active texture 0...
	
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); 
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*8));
		
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
	//glDrawArrays(GL_QUADS, 0, 4);

;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

	//printf("PP Draw 3\n");
}


unsigned int OpenGlTinyTank::LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height)
{
	unsigned int width	= 0;
	unsigned int height = 0;
	std::vector< unsigned char > pixels;

	// populates "pixels" vector with pixel data formated as RGBA
	// width and height are passed in as reference. If the function succeeds, 
	// with and height should be populated with the loaded textures widht and height in pixels
	lodepng::decode( pixels, width, height, filename );


	// TODO:
	//-------------------------------------------------------------------------
	// Procedrally genorate an "ERROR" texture
	// and load it into memory for any textures that could not be found
	//-------------------------------------------------------------------------

	// the texture did not load
	if( pixels.size() == 0)
	{
		width = 128;
		height = 128;

		bool flipFlop = false;
		unsigned int counter = 1;

		for(int x = 0; x < 512; x++)
		{
			if(counter >= 128)//128 is 4 rows across, flip the checker pattern for new row
			{
				counter = 0;
				if(flipFlop)
					flipFlop = false;
				else
					flipFlop = true;
			}			
			if(flipFlop)
			{
				for(int i=0; i < 32; i++)
				{
					pixels.push_back( 97 ); // red
					pixels.push_back( 97 ); // green
					pixels.push_back( 97 ); // blue
					pixels.push_back( 255 ); // alpha
				}				
				flipFlop = false;				
			}
			else
			{
				for(int i=0; i < 32; i++)
				{
					pixels.push_back( 230 ); // red
					pixels.push_back( 230 ); // green
					pixels.push_back( 230 ); // blue
					pixels.push_back( 255 ); // alpha
				}
				flipFlop = true;
			}
			counter++;
		}
	}

	if( out_width	!= NULL )	*out_width	= width;
	if( out_height	!= NULL )	*out_height = height;

	// create an openGL texture
	//-------------------------------------------------------------------------

		unsigned int textureID = 0;
		glGenTextures(1, &textureID); // genorate the texture and store the unique id in textureID

		glBindTexture(GL_TEXTURE_2D, textureID); // make the genorated texture the current texture

		// send the pixel data to the current texture
		// NOTE: we need to tell openGL what the current format of the pixels is in RAM,
		//       we then need to tell it what format we want the pixels to be formatted in within video memory (openGL will do the conversion)
		//		 we also need to tell it the size of each color channel, which is GL_UNSIGNED_BYTE (one byte each for Red, Green, Blue and Alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glBindTexture(GL_TEXTURE_2D, 0);

	//-------------------------------------------------------------------------

	// return the genorated texture ID
	return textureID;
}