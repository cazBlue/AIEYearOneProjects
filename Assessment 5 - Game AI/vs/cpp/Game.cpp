#include "Game.h"

#include "SpriteBatch.h"
#include "Vec2.h"

#include "Graph.h"
#include "PathFinder.h"

#include "Agent.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Game::~Game()
{
//	delete m_spriteBatch;
}

Game::Game(string a_gameName, unsigned int a_width, unsigned int a_height)
{
	m_title = a_gameName;
	m_width = a_width;
	m_height = a_height;

	//set the default double click time
	m_clickTime = 0.5f;
	m_lastClick = 0;
}


void Game::Load()
{
	//Init sprite batch and open gl
	m_spriteBatch = new SpriteBatch(this);
	m_spriteBatch->Init();

//	m_window = m_spriteBatch->m_window;

	m_Graph = new Graph();

	m_pathFinder = new PathFinder();
	m_pathFinder->m_pathFindType = PathFinder::PATH_TYPE_ASTAR;

	updateInterval = Game::PATHUPDATE_BYFRAME;

	//create new random graph
	//this can be turned off for creating custom graph
	GenRndmGraph();

	m_hoverNode = nullptr;
	m_startNode = nullptr;
	m_endNode = nullptr;

	//ghost node to be created and deleted every frame
	m_ghostNode = nullptr;

	/* initialize random seed: */
	srand ((int)time(NULL));

	//create agents
	for(int i = 0; i < 10; i++)
	{
		Agent* myAgent = new Agent(Vec2(640,360), Vec2(0,0), Vec2(0,0), 100, m_pathFinder);

		m_agents.push_back(myAgent);
	}
	

	//TODO
	//graph loading code
}

void Game::Run()
{
	//load up the sprite batch, openGL and other game elements
	Load();





	//placeholder nodes for testing
//	Graph::Node drawOne(Vec2(10,100));
//
//	Graph::Node drawTwo(Vec2(10,10));
//
//	Graph::Node drawThree(Vec2(100,100));
//
//	Graph::Node drawFour(Vec2(200,100));

	//loop until the user closes the window
	m_lastTime = 0;
	m_currentTime = 0;
	m_deltaTime = 0;

	//set time to zero to avoid initial delta time pop
	glfwSetTime (0.00);

	while(!glfwWindowShouldClose(m_spriteBatch->m_window))
	{
		m_lastTime		= m_currentTime;		
		m_currentTime	= (float)glfwGetTime();
		m_deltaTime		= m_currentTime - m_lastTime;

		//update the mouse position
		double mX, mY;
		glfwGetCursorPos(m_spriteBatch->m_window, &mX, &mY);
		m_mPos = Vec2((float)mX, (float)mY);

		//testing line/ node drawing

//		m_spriteBatch->DrawLine(Vec2(10,10), Vec2(10,100));
//
//		m_spriteBatch->DrawLine(Vec2(100,100), Vec2(200,100));
////
////		m_spriteBatch->DrawLine(Vec2(200,100), Vec2(550,0), SPRITE_COLOUR_RED);
////
//		m_spriteBatch->Draw(&drawOne, SPRITE_COLOUR_BLUE);
////
//		m_spriteBatch->Draw(&drawTwo, SPRITE_COLOUR_GREEN);
////
//		m_spriteBatch->Draw(&drawThree, SPRITE_COLOUR_RED);
////
//		m_spriteBatch->Draw(&drawFour);


		//======================== UPDATE

		Update(m_deltaTime);

//		if( glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_UP ) || glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_W ))
//		{
//			m_agent->force.y -= 200.0f;
//		}
//
//		if( glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_DOWN ) || glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_S ))
//		{
//			m_agent->force.y += 200.0f;
//		}

		//TODO make updating things like change path finding types safe using a que. 

		if( glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_P ) && m_currentTime - m_lastClick  >= .3)
		{
			if(m_pathFinder->m_pathFindType == PathFinder::PATH_TYPE_ASTAR)
			{
				m_pathFinder->m_pathFindType = PathFinder::PATH_TYPE_DIJKSTRAS;
			}
			else
				m_pathFinder->m_pathFindType = PathFinder::PATH_TYPE_ASTAR;

			m_lastClick = m_currentTime;
		}

		//update testing agent
		list<Agent*>::iterator iter;
		for(iter = m_agents.begin(); iter != m_agents.end(); iter++)
		{
			(*iter)->Update(m_deltaTime, m_mPos);
		}

		//=========================

		//======================== DRAW

		Draw();

		//=========================
	}
}


//========================== RUN / DRAW

void Game::Draw()
{
		m_spriteBatch->Begin();
		
		SPRITE_COLOUR col;

		//draw edges
		list<Graph::Node*>::iterator iter;		
		for(iter = m_Graph->m_nodes.begin(); iter != m_Graph->m_nodes.end(); iter++)
		{
			list<Graph::Edge*>::iterator iter2;	
			for(iter2 = (*iter)->m_edges.begin(); iter2 != (*iter)->m_edges.end(); iter2++)
			{
				col = SPRITE_COLOUR_WHITE;

				m_spriteBatch->DrawLine( (*iter)->m_pos , (*iter2)->m_connection->m_pos, col);
			}
		}


		//draw ghost node edges if ghost node is active
		if(m_ghostNode != nullptr && m_ghostNode->m_edges.size() > 0)
		{
			list<Graph::Edge*>::iterator iterGhost;	
			for(iterGhost = m_ghostNode->m_edges.begin(); iterGhost != m_ghostNode->m_edges.end(); iterGhost++)
			{
				col = SPRITE_COLOUR_BLUE;

				m_spriteBatch->DrawLine( m_ghostNode->m_pos , (*iterGhost)->m_connection->m_pos, col);
			}
		}

		//draw nodes in graph
		for(iter = m_Graph->m_nodes.begin(); iter != m_Graph->m_nodes.end(); iter++)
		{
			col = SPRITE_COLOUR_WHITE;

			if((*iter) == m_hoverNode)
				col = SPRITE_COLOUR_BLUE;
			if((*iter) == m_startNode)
				col = SPRITE_COLOUR_GREEN;
			if((*iter) == m_endNode)
				col = SPRITE_COLOUR_RED;

			m_spriteBatch->Draw((*iter), col);
		}


		//if there is a known path draw it
		if(m_pathFinder->IsPathFound())
		{
			
			Vec2 lastPos = (*m_pathFinder->m_Path.begin())->m_pos;
		

			vector<Graph::Node*>::iterator iterPath;	
			for(iterPath = m_pathFinder->m_Path.begin(); iterPath != m_pathFinder->m_Path.end(); iterPath++)
			{
				if((*iterPath) == m_startNode)
					int temp = 0;

				col = SPRITE_COLOUR_GREEN;

				Vec2 pos = (*iterPath)->m_pos;

				m_spriteBatch->DrawLine(lastPos, pos, col);

				lastPos = pos;
			}
		}



		list<PathFinder::Node*>::iterator iterPathNodes;

		//draw nodes in open list
		for(iterPathNodes = m_pathFinder->m_OpenList.begin(); iterPathNodes != m_pathFinder->m_OpenList.end(); iterPathNodes++)
		{
			m_spriteBatch->Draw((*iterPathNodes)->m_node, SPRITE_COLOUR_RED);
		}

		//draw nodes in closed list
		for(iterPathNodes = m_pathFinder->m_ClosedList.begin(); iterPathNodes != m_pathFinder->m_ClosedList.end(); iterPathNodes++)
		{
			m_spriteBatch->Draw((*iterPathNodes)->m_node, SPRITE_COLOUR_GREEN);
		}

		//draw the ghost node if active
		if(m_ghostNode != nullptr)
			m_spriteBatch->Draw(m_ghostNode);




		//=-============== draw agents

				//update testing agent
		list<Agent*>::iterator iterAgent;
		for(iterAgent= m_agents.begin(); iterAgent != m_agents.end(); iterAgent++)
		{			
			m_spriteBatch->DrawAgent((*iterAgent));
		}

		


		//program visual control prompts

		string pathType;
		stringstream builder;
		string agentState;

		switch (m_pathFinder->m_pathFindType)
		{
		case PathFinder::PATH_TYPE_DIJKSTRAS:
			pathType = "Dijkstras";
			break;
		case PathFinder::PATH_TYPE_ASTAR:
			pathType = "AStar";
			break;
		default:
			break;
		}

		builder << "Path algorithm: " << pathType;

		pathType = builder.str();

		m_spriteBatch->DrawText("Right click to add nodes", 10, 10, .4);

		m_spriteBatch->DrawText("Left click nodes to create path", 10, 30, .4);

		m_spriteBatch->DrawText("Left click empty space to clear path", 10, 50, .4);




		m_spriteBatch->DrawText("Toggle path type: p", 10, 100, .4);

		m_spriteBatch->DrawText(pathType.c_str(), 10,120, .4);

		if(m_pathFinder->IsPathFound())
			agentState = "Agent state: following path - Seek";
		else
			agentState = "Agent state: Wander";

		m_spriteBatch->DrawText(agentState.c_str(), 10, 200, .4);


		


		string updateType;

		switch (updateInterval)
		{
		case Game::PATHUPDATE_BYFRAME:
				updateType = "UPDATE TYPE: By frame";
			break;
		case Game::PATHUPDATE_INSTANT:
				updateType = "UPDATE TYPE: Instant";
			break;
		case Game::PATHUPDATE_MANUAL:
				updateType = "UPDATE TYPE: Manual (press spacebar)";
			break;
		default:
			break;
		}
		m_spriteBatch->DrawText("Toggle path update type: u", 10, 250, .4);

		m_spriteBatch->DrawText(updateType.c_str() , 10, 270, .4);

		m_spriteBatch->End();
}

void Game::Update(float a_dt)
{

		//check if currently pathfinding and run iteration
	if(m_pathFinder->RequiresUpdate())
	{
		if(updateInterval == Game::PATHUPDATE_BYFRAME)
			m_pathFinder->Update();				

		if(updateInterval == Game::PATHUPDATE_MANUAL)	
		{
			if(glfwGetKey(m_spriteBatch->m_window, GLFW_KEY_SPACE) && m_currentTime - m_lastClick  >= .3)
			{
				m_pathFinder->Update();			
				m_lastClick = m_currentTime;
			}
		}

		if(updateInterval == Game::PATHUPDATE_INSTANT)	
		{
			while(m_pathFinder->RequiresUpdate())
				m_pathFinder->Update();	
		}
		

		//manual update frames


	}

	//end path finding

	if( glfwGetKey( m_spriteBatch->m_window, GLFW_KEY_U ) && m_currentTime - m_lastClick  >= .3)
	{
		switch (updateInterval)
		{
		case Game::PATHUPDATE_BYFRAME:
				updateInterval = Game::PATHUPDATE_INSTANT;
			break;
		case Game::PATHUPDATE_INSTANT:
				updateInterval = Game::PATHUPDATE_MANUAL;
			break;
		case Game::PATHUPDATE_MANUAL:
				updateInterval = Game::PATHUPDATE_BYFRAME;
			break;
		default:
			break;
		}
		
		m_lastClick = m_currentTime;
	}

		//----------------------------------- set hover node
		//get list of nodes near the mouse
		list<Graph::Node*> nodesNearMouse = m_Graph->FindNodesInRange(m_mPos, 8);

		m_hoverNode = nullptr;

		if(nodesNearMouse.size() > 0)
			m_hoverNode = nodesNearMouse.back();

		
		//-------------------------------- set start/end node
		//check if we have clicked and if 
		if(glfwGetMouseButton(m_spriteBatch->m_window, GLFW_MOUSE_BUTTON_1) &&  m_currentTime - m_lastClick  >= m_clickTime )
		{
			//if start node has not been selected and there is a hover node (mouse is in range if so)
			if(m_startNode == nullptr && m_hoverNode != nullptr)
				m_startNode = m_hoverNode;	//start node takes over hover node
			else if(m_endNode == nullptr && m_hoverNode != nullptr)
			{
				if(m_hoverNode != m_startNode)
				{
					m_endNode = m_hoverNode;

					m_pathFinder->BeginPathFinding(m_startNode, m_endNode, m_pathFinder->m_pathFindType);
				}
			}
			else if(m_hoverNode == nullptr)
			{				
				m_pathFinder->Reset();

				m_endNode = nullptr;
				m_startNode = nullptr;
			}

			m_lastClick = m_currentTime;
		}

		//add node on right mouse click
		if(glfwGetMouseButton(m_spriteBatch->m_window, GLFW_MOUSE_BUTTON_2) &&  m_currentTime - m_lastClick  >= m_clickTime )
		{
			//add node
			m_Graph->AddNode(m_mPos);

			//clear the edge lists out of every node for rebuilding
			list<Graph::Node*>::iterator iter1;
			for(iter1 = m_Graph->m_nodes.begin(); iter1 != m_Graph->m_nodes.end(); iter1++)
			{
				(*iter1)->m_edges.clear();
			}			

			//rebuild all edge connections
			ConEdges();			

			m_lastClick = m_currentTime;
		}


		//update the ghost node only if there is no hover node
		//if there is a hover node deactivate the ghost node
		if(m_hoverNode == nullptr)
			GhostUpdate();
		else
			m_ghostNode = nullptr;
}




//============================ END RUN/ DRAW



void Game::GenRndmGraph()
{

	float spacing = 96;

	int numX = (int)(m_width / spacing);
	int numY = (int)(m_height / spacing);


	//create nodes
	for(int y = 0; y < numY; y++)
	{
		for(int x = 0; x < numX; x++)
		{
			int rand1 = rand() % 50 + 1;
			int rand2 = rand() % 50 + 1;

			//get random x/y position
			float xOffset = (spacing / 3) + (x * spacing) + rand1; //* (spacing / 4);
			float yOffset = (spacing / 3) + (y * spacing) + rand2; //* (spacing / 4);

			//make sure graph isn't under the text
			if(xOffset > 350 || yOffset > 300)
				m_Graph->AddNode(Vec2(xOffset, yOffset));
		}
	}


	//add edges
	ConEdges();
}


void Game::ConEdges()
{
	float spacing = 96;
	
	//create edges
	list<Graph::Node*>::iterator iter1;
	list<Graph::Node*>::iterator iter2;

	for(iter1 = m_Graph->m_nodes.begin(); iter1 != m_Graph->m_nodes.end(); iter1++)
	{
		for(iter2 = m_Graph->m_nodes.begin(); iter2 != m_Graph->m_nodes.end(); iter2++)
		{
			if((*iter1) == (*iter2))
				continue;

			float length = ((*iter1)->m_pos - (*iter2)->m_pos).Length();

			if(length <= spacing * 1.5)
			{
				m_Graph->AddConnection((*iter1), (*iter2), length);
			}
		}		
	}
}

void Game::GhostUpdate()
{

	float spacing = 96;


	if(m_ghostNode != nullptr)
		m_ghostNode = nullptr;
	
	//create new ghost node and set the position
	m_ghostNode = new Graph::Node(m_mPos);
	
	list<Graph::Node*>::iterator iter2;
	//check ghost node against all nodes
	for(iter2 = m_Graph->m_nodes.begin(); iter2 != m_Graph->m_nodes.end(); iter2++)
	{
		float length = (m_ghostNode->m_pos - (*iter2)->m_pos).Length();

		if(length <= spacing * 1.5)
		{
			m_Graph->AddConnection(m_ghostNode, (*iter2), length);
		}
	}
}