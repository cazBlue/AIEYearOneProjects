#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <glew.h>


class Texture;

enum TEX_ID
{
	TEX_ID_PLAYER_TANK,
	TEX_ID_PLAYER_TURRET,
	TEX_ID_ENEMY_TANK,
	TEX_ID_ENEMY_TURRET,
	TEX_ID_BULLET,
	TEX_ID_PLAYER_BULLET,
	TEX_ID_ENEMY_BULLET,
};

struct texInfo
{
	unsigned int m_width, m_height;

	GLuint m_VBO, m_IBO, m_ui_TextureId;
};

class TexMngr
{
public:
	
	//default contsructor
	TexMngr();

	//load all textures
	void LoadCommonTextures();

	static unsigned int LoadCustomTexture(const char *filename, unsigned int *out_width, unsigned int *out_height);

	//get a texture for newly created actors
	texInfo* GetTexture(TEX_ID a_textToGet);

private:
	//-----------------load individual textures
	void LoadPlayerTank();
	void LoadPlayerTurret();
	void LoadEnemyTank();
	void LoadEnemyTurret();
	void LoadBullet();
	void LoadPlayerBullet();
	void LoadEnemyBullet();
	void LoadCHECKER();

	texInfo m_checker;

	texInfo m_player_tank;
	texInfo m_player_Turret;

	texInfo m_enemy_tank;
	texInfo m_enemy_turret;

	texInfo m_bullet;
	texInfo m_bullet_player;
	texInfo m_bullet_enemy;

	//TODO have a list of textures that can be assigned at random to bullets/ enemies etc
};

#endif