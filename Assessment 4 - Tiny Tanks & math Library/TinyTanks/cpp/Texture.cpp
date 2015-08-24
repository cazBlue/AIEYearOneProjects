#include "Texture.h"

#include "lodepng.h"

#include "TextureManager.h"

Texture::~Texture()
{
//	if(m_textureManager)
//		delete m_textureManager;
}


Texture::Texture(TexMngr* a_texManager, TEX_ID a_texID)
{

	//loads info from a pre-loaded texture
	
	m_textureManager = a_texManager;

	texInfo* myTex = m_textureManager->GetTexture(a_texID);

	m_VBO = myTex->m_VBO;
	m_IBO = myTex->m_IBO;
	m_ui_TextureId = myTex->m_ui_TextureId;
	m_width = myTex->m_width;
	m_height = myTex->m_height;

	m_ui_TextureId = myTex->m_ui_TextureId;

//	delete myTex;
}

Texture::Texture(const char *fileName)
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//m_textureManager = a_texManager;

	//load texture
	m_ui_TextureId = Texture::LoadTexture(fileName, &m_width, &m_height);	
}

unsigned int Texture::getHeight()
{
	return m_height;
}

unsigned int Texture::getWidth()
{
	return m_width;
}

void Texture::setHeight(unsigned int a_height)
{
	m_height = a_height;
}

void Texture::setWidth(unsigned int a_width)
{
	m_width = a_width;
}

void Texture::setWidthHeight(unsigned int a_width, unsigned int a_height)
{
	m_width = a_width;
	m_height = a_height;
}

unsigned int Texture::LoadTexture(const char *filename, unsigned int *out_width, unsigned int *out_height)
{
	//load a custom texture
	return TexMngr::LoadCustomTexture(filename, out_width, out_height);
}