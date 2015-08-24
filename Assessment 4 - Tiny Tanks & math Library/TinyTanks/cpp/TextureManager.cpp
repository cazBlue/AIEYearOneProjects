#include "TextureManager.h"


#include <iostream>


#include "lodepng.h"

TexMngr::TexMngr()
{

}

void TexMngr::LoadCommonTextures()
{
	std::cout << "Loading Common Texures " << std::endl;

	LoadPlayerTank();
	LoadPlayerTurret();
	LoadEnemyTank();
	LoadEnemyTurret();
	LoadBullet();
	LoadPlayerBullet();
	LoadEnemyBullet();
}

void TexMngr::LoadPlayerTurret()
{
	glGenBuffers(1, &m_player_Turret.m_VBO);
	glGenBuffers(1, &m_player_Turret.m_IBO);
	m_player_Turret.m_ui_TextureId = LoadCustomTexture("./content/images/master_Tank_Turret.png", &m_player_Turret.m_width, &m_player_Turret.m_height);
}

void TexMngr::LoadPlayerTank()
{
	glGenBuffers(1, &m_player_tank.m_VBO);
	glGenBuffers(1, &m_player_tank.m_IBO);
	m_player_tank.m_ui_TextureId = LoadCustomTexture("./content/images/master_Tank_Base.png", &m_player_tank.m_width, &m_player_tank.m_height);
}

void TexMngr::LoadEnemyTank()
{
	glGenBuffers(1, &m_enemy_tank.m_VBO);
	glGenBuffers(1, &m_enemy_tank.m_IBO);
	m_enemy_tank.m_ui_TextureId = LoadCustomTexture("./content/images/master_Tank_Base.png", &m_enemy_tank.m_width, &m_enemy_tank.m_height);
}

void TexMngr::LoadEnemyTurret()
{
	glGenBuffers(1, &m_enemy_turret.m_VBO);
	glGenBuffers(1, &m_enemy_turret.m_IBO);
	m_enemy_turret.m_ui_TextureId = LoadCustomTexture("./content/images/master_Tank_Turret.png", &m_enemy_turret.m_width, &m_enemy_turret.m_height);
}

void TexMngr::LoadBullet()
{
	glGenBuffers(1, &m_bullet.m_VBO);
	glGenBuffers(1, &m_bullet.m_IBO);
	m_bullet.m_ui_TextureId = LoadCustomTexture("./content/images/Master_Tank_Shell.png", &m_bullet.m_width, &m_bullet.m_height);
}

void TexMngr::LoadPlayerBullet()
{
	//currently using bullet only
}

void TexMngr::LoadEnemyBullet()
{
	//currently using bullet only
}

void TexMngr::LoadCHECKER()
{
}

texInfo* TexMngr::GetTexture(TEX_ID a_textToGet)
{
	switch (a_textToGet)
	{
	case TEX_ID_PLAYER_TANK:
		return &m_player_tank;
		break;
	case TEX_ID_PLAYER_TURRET:
		return &m_player_Turret;
		break;
	case TEX_ID_ENEMY_TANK:
		return &m_enemy_tank;
		break;
	case TEX_ID_ENEMY_TURRET:
		return &m_enemy_turret;
		break;
	case TEX_ID_BULLET:
		return &m_bullet;
		break;
	case TEX_ID_PLAYER_BULLET:
		return &m_bullet;
		//return &m_bullet_player;
		break;
	case TEX_ID_ENEMY_BULLET:
		return &m_bullet;
		//return &m_bullet_enemy;
		break;
	default:
		return &m_checker;
		break;
	}
}



unsigned int TexMngr::LoadCustomTexture(const char *filename, unsigned int *out_width, unsigned int *out_height)
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

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glBindTexture(GL_TEXTURE_2D, 0);

	//-------------------------------------------------------------------------

	// return the generated texture ID
	return textureID;
}