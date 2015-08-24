#include "SpriteBatch.h"
#include "Game.h"
#include <list>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "lodepng.h"
#include "Mat3.h"
#include "Vec2.h"
#include "Agent.h"
//#include "Graph.h"
using namespace std;


SpriteBatch::SpriteBatch(Game* a_game)
{
	m_game = a_game;
}

//------------------------ COMMON DRAW FUNCTIONS	--------------------------------------

void SpriteBatch::Begin()
{
	//Clear the buffer
	glClearColor(0.4f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void SpriteBatch::DrawText(const char *text, float x, float y, float a_scale) 
{

	//TODO 
	// 1- allow different fonts to be selected
	// 2- allow a max width to be defined, if text goes over that width scale it down

	glUseProgram(m_uiTexProgram);

	glUniformMatrix4fv(m_MatrixIDFlat, 1, GL_FALSE, m_orthoProjection);
//	std::cout << glGetError() << std::endl;

	//TODO make font colour settable by passing in rgba
	//set the font colour
	GLfloat fColour[4] = {0, 0, 0, 1};

	//get the colour value from the shader
	GLint textureLocation = glGetUniformLocation(m_uiTexProgram, "color");
	if (textureLocation == -1) {
	   fprintf(stderr, "Could not bind uniform %s\n", "color");
	}

	glUniform4fv(textureLocation, 1, fColour);	
	
	const char *p;

  for(p = text; *p; p++) 
	{
		if(FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
			continue;
 
		glTexImage2D(
		  GL_TEXTURE_2D,
		  0,
		  GL_ALPHA,
		  m_glyph->bitmap.width,
		  m_glyph->bitmap.rows,
		  0,
		  GL_ALPHA,
		  GL_UNSIGNED_BYTE,
		  m_glyph->bitmap.buffer
		);
 		
		// aligning the font based on glyph origin was eluding me, final code based on answer found here: http://stackoverflow.com/questions/22868070/rendering-freetype-fonts
		//this was the missing height offset needed to push each character down to it's correct origin point
		float hOffset = m_glyph->face->size->metrics.height / 64 * a_scale;

		float x2 = x +	m_glyph->bitmap_left *		a_scale;
		float y2 =		m_glyph->bitmap_top *		a_scale;
		float w =		m_glyph->bitmap.width *		a_scale;
		float h =		m_glyph->bitmap.rows *		a_scale;

//		float yPos = newH - heightOffset;

		//height offset to align to chose y pos
		float yOff = -hOffset * .16;
		
		GLfloat box[4][4] = {
			{x2,						yOff + y + -y2		+ hOffset,			0, 0},
			{x2 + w,					yOff + y + -y2		+ hOffset,			1, 0},
			{x2,						yOff + y + h - y2	+ hOffset,			0, 1},
			{x2 + w,					yOff + y + h - y2	+ hOffset,			1, 1},
		};

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


			// bind our texture to active texture 0...
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, a_texID);

		glBindBuffer(GL_ARRAY_BUFFER, m_tex_VBO);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Text_textureID);

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		
		//or 0 as attrib is at location 0
		int attribLocation = glGetAttribLocation(m_uiTexProgram, "coord");
		glEnableVertexAttribArray(attribLocation);
		//std::cout << glGetError() << std::endl;
		
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
		x += (m_glyph->advance.x >> 6) * a_scale;
		y += (m_glyph->advance.y >> 6) * a_scale;
		
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
  }


	glUseProgram(0);
}


void SpriteBatch::DrawCircle(float cx, float cy, float r) 
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

	glColor3f(1,0,0);
	//TODO move program creation to main game to avoid creating a new program every from
	//thats causing lag when debugging

	glUseProgram(m_uiProgram);

	glUniformMatrix4fv(m_MatrixIDFlat, 1, GL_FALSE, m_orthoProjection);

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

void SpriteBatch::DrawAgent(Agent* a_agent)
{

	Vec2 pos = a_agent->m_pos;

	float width		= (float)m_agentWidth;
	float height	= (float)m_agentHeight;

	int xOff = (int)width	/ 2;
	int yOff = (int)height	/ 2;

	Vec2 tl = Vec2(-xOff,			-yOff);
	Vec2 tr = Vec2(xOff ,			-yOff);
	Vec2 br = Vec2(-xOff,			yOff);
	Vec2 bl = Vec2(xOff ,			yOff);	

	float rot = Vec2(0,1).GetAngleBetween(a_agent->m_heading);

	//create matrix for start point
	Mat3 rotMat; 	
	rotMat.Rotate(rot + 3.141592654);
	rotMat.SetTranslation(pos);


	//set the 4 vert points to correct rotation
	tl = rotMat.TransformPoint(tl);
	tr = rotMat.TransformPoint(tr);
	br = rotMat.TransformPoint(br);
	bl = rotMat.TransformPoint(bl);	


	processSprite(&tl, &tr, &bl, &br, m_agentIBO, m_agentVBO, m_texID_agent, SPRITE_COLOUR_WHITE);
}

//draw node
void SpriteBatch::Draw(Graph::Node* a_Node, SPRITE_COLOUR a_col)
{
	//passed node pos is considered the centre

	//nodes do not rotate, no matrix or other transform needed

	float width		= (float)m_node_width;
	float height	= (float)m_node_height;

	int xOff = (int)width	/ 2;
	int yOff = (int)height	/ 2;

	Vec2 pos = a_Node->m_pos;
	
	Vec2 tl = Vec2(-xOff + pos.x,			pos.y - yOff);
	Vec2 tr = Vec2(xOff + pos.x,			pos.y - yOff);
	Vec2 br = Vec2(-xOff + pos.x,			pos.y + yOff);
	Vec2 bl = Vec2(xOff + pos.x,			pos.y + yOff);		
	
	processSprite(&tl, &tr, &bl, &br, m_nodeIBO, m_nodeVBO, m_texID_node, a_col);
}

//Draw Line
void SpriteBatch::DrawLine(Vec2 a_start, Vec2 a_end, SPRITE_COLOUR a_col, float a_width)
{
	//width used to offsetting
	float width;

	if(a_width > 0)
		width = a_width;
	else
		width = (float)m_line_width / 2;	

	//width override for debugging
	//width = 50;

	float size = (a_start - a_end).Length();

	Vec2 origin = Vec2(-width / 2, 0);

	//set the line around the zero point with the pivot
	//at the top center (0,0)
	//oriented for rotation zero along the x axis
	Vec2 tl = Vec2(0,					0) + origin;
	Vec2 tr = Vec2(width,				0) + origin;
	Vec2 br = Vec2(0,				size)  + origin;
	Vec2 bl = Vec2(width,			size)  + origin;

	//get difference normalised
//	Vec2 diff = Vec2(a_end.x - a_start.x, a_end.y - a_start.y).GetNormalised();

	Vec2 diff = (a_start - a_end).GetNormalised();


	float rot = Vec2(0,1).GetAngleBetween(diff);

	//create matrix for start point
	Mat3 rotMat; 	
	rotMat.Rotate(rot + 3.141592654);
	rotMat.SetTranslation(a_start);
		

	//transform line start point
	tl = rotMat.TransformPoint(tl);
	tr = rotMat.TransformPoint(tr);

	//transform line end point
	br = rotMat.TransformPoint(br);
	bl = rotMat.TransformPoint(bl);

	processSprite(&tl, &tr, &bl, &br, m_lineIBO, m_lineVBO, m_texID_line, a_col);
}


void SpriteBatch::processSprite(Vec2* a_tl, Vec2* a_tr, Vec2* a_bl, Vec2* a_br, GLuint a_IBO, GLuint a_VBO, unsigned int a_texID, SPRITE_COLOUR a_col)
{
	struct col
	{
		float r;
		float g;
		float b;
	};

	col sCol;

	switch (a_col)
	{
	case SPRITE_COLOUR_WHITE:
		sCol.r = 1.0f;
		sCol.g = 1.0f;
		sCol.b = 1.0f;
		break;
	case SPRITE_COLOUR_RED:
		sCol.r = 1.0f;
		sCol.g = 0.0f;
		sCol.b = 0.0f;
		break;
	case SPRITE_COLOUR_GREEN:
		sCol.r = 0.0f;
		sCol.g = 1.0f;
		sCol.b = 0.0f;
		break;
	case SPRITE_COLOUR_BLUE:
		sCol.r = 0.0f;
		sCol.g = 0.0f;
		sCol.b = 1.0f;
		break;
	default:
		break;
	}

	Vertex vertexData[4] = 
	{
		Vertex(a_tl->x, a_tl->y,	sCol.r, sCol.g, sCol.b, 1.0f,		 0.0f, 0.0f),
		Vertex(a_tr->x, a_tr->y,	sCol.r, sCol.g, sCol.b, 1.0f,		 1.0f, 0.0f),
		Vertex(a_bl->x, a_bl->y,	sCol.r, sCol.g, sCol.b, 1.0f,		 1.0f, 1.0f),
		Vertex(a_br->x, a_br->y,	sCol.r, sCol.g, sCol.b, 1.0f,		 0.0f, 1.0f)
	};

	GLubyte indices[6] = 
	{
		0, 1, 2,	// first triangle
		0, 2, 3		// second triangle
	};

	glUseProgram(m_uiProgram);

	//std::cout << glGetError() << std::endl;

	// bind our texture to active texture 0...
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_texID);
	int textureLocation = glGetUniformLocation(m_uiProgram, "myTextureSampler");
	glUniform1i(textureLocation , 1 );
	
	//send our orthographic projection info to the shader
	glUniformMatrix4fv(m_MatrixIDFlat, 1, GL_FALSE, m_orthoProjection);
	

	glBindBuffer(GL_ARRAY_BUFFER, a_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_IBO);
	
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
}

//*********** getting the angle between 2 vectors and then getting the rotation from on vector to another

	//------------get rotation angle from vecA to vecB

	//get direction
//	Vec2 dir = Vec2(a_end.x - a_start.x, a_start.y - a_end.y);
//
//	//get magnitude
//	float dirMag = sqrt(( dir.x * dir.x) + (dir.y * dir.y));
//
//	//normalise direction
//	dir = Vec2(dir.x / dirMag, dir.y / dirMag);
//
//	float rot = angleBetween(Vec2(0,1), dir);
//
//
//	cout << "the rotation: " << rot << endl;

//float SpriteBatch::angleBetween(Vec2 a_start, Vec2 a_end)
//{
//
////--------- get rotation between vectors
//	
//	//get magnitudes
//	float aMag =  sqrt(a_start.x * a_start.x + a_start.y * a_start.y);
//	float bMag =  sqrt(a_end.x * a_end.x + a_end.y * a_end.y);
//
//	//get normalised Vectors
//	Vec2 aNorm = Vec2( a_start.x / aMag, a_start.y / aMag);
//	Vec2 bNorm = Vec2( a_end.x / bMag, a_end.y / bMag);
//
//	//get the perpindicular
//	Vec2 PerpA = Vec2(aNorm.y, -aNorm.x);
//
//	//get dot product of start and end
//	float aDb = (aNorm.x * bNorm.x) + (aNorm.y * bNorm.y);
//	
//	//dot product of the perpendicular
//	float PerpADotB = (PerpA.x * bNorm.x) + (PerpA.y * bNorm.y);
//
//	float angle = sin(aDb);
//
//	if(PerpADotB > 0)
//		angle *= -1;
//
//	return angle;
//}



void SpriteBatch::End()
{
	//swap front and back buffers
	glfwSwapBuffers(m_window);
		
	//poll for and process events
	glfwPollEvents();
}



//------------------------ SETUP & INIT FUNCTIONS --------------------------------------


unsigned int SpriteBatch::LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height)
{
	unsigned int width	= 0;
	unsigned int height = 0;
	std::vector< unsigned char > pixels;

	// populates "pixels" vector with pixel data formated as RGBA
	// width and height are passed in as reference. If the function succeeds, 
	// with and height should be populated with the loaded textures widht and height in pixels
	lodepng::decode( pixels, width, height, filename );



	// the texture did not load
	//generate a checkerboard texture
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
					pixels.push_back( 127 ); // alpha
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
					pixels.push_back( 127 ); // alpha
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

		//enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glBindTexture(GL_TEXTURE_2D, textureID); // make the genorated texture the current texture

		// send the pixel data to the current texture
		// NOTE: we need to tell openGL what the current format of the pixels is in RAM,
		//       we then need to tell it what format we want the pixels to be formatted in within video memory (openGL will do the conversion)
		//		 we also need to tell it the size of each color channel, which is GL_UNSIGNED_BYTE (one byte each for Red, Green, Blue and Alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);		


		//image blending info reference https://open.gl/textures
		//opengl reference docs https://www.opengl.org/sdk/docs/man/

		//scaling image down blending
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//scaling image up blending
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

	//-------------------------------------------------------------------------

	// return the generated texture ID
	return textureID;
}




//Vec2::Vec2(float a_x, float a_y)
//{
//	x = a_x;
//	y = a_y;
//}


void SpriteBatch::Init()
{
	cout << "Loading Game" << endl;

	m_width = m_game->m_width;
	m_height = m_game->m_height;

	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 5);
	//disable glfw debugging
//	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);

	

	//Initialise GLFW
	if(!glfwInit())
	{
		//return -1;
		exit(1);
	}

	//create a windowed mode window and it's OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, m_game->m_title.c_str(), NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		exit(1);
	}

	//make the window's context current
	glfwMakeContextCurrent(m_window);

	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		exit(1);
		//return -1;
	}

	cout << "glsl version: ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION)  << endl; 

	cout << "Creating program" << endl;
	//create shader program
	m_uiProgram = CreateProgram("./content/shaders/VertexShader.glsl", "./content/shaders/TexturedFragmentShader.glsl");

	cout << "Creating projection matrix" << endl;
	//find the position of the matrix variable in the shader so we can send info there later
	m_MatrixIDFlat = glGetUniformLocation(m_uiProgram, "MVP");	
	//m_MatrixIDFlat_TextOnly = glGetUniformLocation(m_uiTexProgram, "MVP");

	m_orthoProjection = new float[16];
	//set up an ortho at the top left and with 100 levels of z deth
	BuildOrthoProjectionMatrix(m_orthoProjection, 0, (float)m_width, (float)m_height, 0, 0, 100);

	//set the call back to enable esc to exit program
	glfwSetKeyCallback(m_window, key_callback);

	//TODO replace with proper texture manager

	//gen line VBO/IBO
	glGenBuffers(1, &m_lineVBO);
	glGenBuffers(1, &m_lineIBO);
	//load line texture
	m_texID_line = LoadTexture( "./content/images/edge_plain.png", &m_line_width, &m_line_height);
	//gen node VBO/IBO
	glGenBuffers(1, &m_nodeVBO);
	glGenBuffers(1, &m_nodeIBO);

	m_texID_node = LoadTexture( "./content/images/Node.png", &m_node_width, &m_node_height);

	glGenBuffers(1, &m_agentVBO);
	glGenBuffers(1, &m_agentIBO);

	m_texID_agent = LoadTexture( "./content/images/Agent.png", &m_agentWidth, &m_agentHeight);


	//=============================					Load fonts
	cout << "setting up fonts" << endl;

	if(FT_Init_FreeType(&m_ft)) {
	  fprintf(stderr, "Could not init freetype library\n");	 
	}	
 
	if(FT_New_Face(m_ft, "./content/fonts/arial.ttf", 0, &m_face)) {
	  fprintf(stderr, "Could not open font\n");	  
	}
	
	FT_Set_Pixel_Sizes(m_face, 0, 48);

	if(FT_Load_Char(m_face, 'X', FT_LOAD_RENDER)) {
	  fprintf(stderr, "Could not load character 'X'\n");
	}


	m_glyph = m_face->glyph;


	//==================			Load text elements
	m_uiTexProgram = CreateProgram("./content/shaders/VertTextShader.glsl", "./content/shaders/FragTextShader.glsl");
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_Text_textureID);

	//enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, m_Text_textureID);


	glGenBuffers(1, &m_tex_VBO);
//	glEnableVertexAttribArray(attribLocation);
	//std::cout << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, (m_tex_VBO));
//	std::cout << glGetError() << std::endl;

	//reset the texture bind
	glBindTexture(GL_TEXTURE_2D, 0);
}




void SpriteBatch::BuildOrthoProjectionMatrix( float *pMat4x4, float left, float right, float bottom, float top, float a_near, float a_far) 
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

GLuint SpriteBatch::CreateProgram(const char *a_vertex, const char *a_frag)
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

GLuint SpriteBatch::CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
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

	GLint Result = GL_FALSE;


	return uiShader;
}




//----------------------   callbacks
void SpriteBatch::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "key pressed" << endl;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void SpriteBatch::error_callback(int error, const char* description)
{
    fputs(description, stderr);
}