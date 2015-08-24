#ifndef TEXTURE_H
#define TEXTURE_H

#include <glew.h>


/*
	Textures are mainly controlled via the textre managers

	The texture class retains the ability to load textures for cases
	when the texture is not in the manager

	Textres inialised through the manager should have all needed settings initlaised
*/

class TexMngr;
enum TEX_ID;

class Texture
{
public:
	//blank constructor, used when controlled by texture manager
	Texture();

	//TODO make "animated" texture option

	//load a custom texture
	Texture(const char *fileName);

	//use a pre loaded texture
	Texture(TexMngr* a_texManager, TEX_ID a_texID);
	
	//deconstructor
	~Texture();

	//get/set height of texture in pixels
	unsigned int getHeight();
	void setHeight(unsigned int a_height);
	//get/set width of texture in pixels
	unsigned int getWidth();
	void setWidth(unsigned int a_width);

	void setWidthHeight(unsigned int a_width, unsigned int a_height);

	friend class SpriteBatch;
	


private:
	//Open gl buffer slots
	GLuint m_VBO, m_IBO, m_ui_TextureId;
	
	//static unsigned int LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height);
	static unsigned int LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height);

	unsigned int m_width, m_height;

	TexMngr* m_textureManager;
};


#endif