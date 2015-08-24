#include "PlayerTank.h"
#include "Game.h"

#include <iostream>

#include "TextureManager.h"

PlayerTank::PlayerTank(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , const char *textureFileName, Mat3 startLocation, Vec2 a_pivot, Actor *a_parent) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho,  textureFileName, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_TANK;
}


PlayerTank::PlayerTank(Game *a_game, GLuint *a_program, GLuint *a_MatrixIDFlat, float *a_ortho , Mat3 startLocation, Vec2 a_pivot, Actor *a_parent) 
					: Actor(a_game, a_program, a_MatrixIDFlat,  a_ortho, startLocation, a_pivot, a_parent)
{
	m_classID = CLASSID_A_TANK;

	m_texture = new Texture(m_mainGame->getTextMngr(), TEX_ID_PLAYER_TANK);
}


void PlayerTank::update(float a_DT)
{
	//Actor::update(); //call to actor update if needed, say for sprite animation
	
//	std::cout << (*m_node->manipMat3()).GetTranslation().x << " (----) " << (*m_node->manipMat3()).GetTranslation().y  << std::endl;
	//rotate turret manually if alternate control scheme is used
	
	float rotSpeed = 10.174532925;
	float moveSpeed = 1;

	
	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_D ) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_RIGHT ))
	{
		Mat3 rotMat = Mat3::CreateRotation(rotSpeed * a_DT);

		//local rotation is rot * matrix, global is mat * rot
		(*m_node->manipMat3()) = rotMat * (*m_node->manipMat3());
	}

	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_A ) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_LEFT ))
	{
		Mat3 rotMat = Mat3::CreateRotation(-rotSpeed * a_DT);

		//local rotation is rot * matrix, global is mat * rot
		(*m_node->manipMat3()) = rotMat * (*m_node->manipMat3());
	}


	//TODO create alternate controls that arn't based on parent rotations

	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_DOWN ) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_S ))
	{
		(*m_node->manipMat3()) = (*m_node->manipMat3()).CreateTranslation(0,(moveSpeed * 100) * a_DT) * (*m_node->manipMat3());
	}	

	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_UP) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_W) )
	{
		(*m_node->manipMat3()) = (*m_node->manipMat3()).CreateTranslation(0,(moveSpeed * -100) * a_DT) * (*m_node->manipMat3());
	}

//	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_LEFT) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_A) )
//	{
//		(*m_node->manipMat3()) = (*m_node->manipMat3()).CreateTranslation(-1,0) * (*m_node->manipMat3());
//	}

//	if( glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_RIGHT ) || glfwGetKey( m_mainGame->getWindow(), GLFW_KEY_D))
//	{
//		(*m_node->manipMat3()) = (*m_node->manipMat3()).CreateTranslation(1,0) * (*m_node->manipMat3());
//	}
}