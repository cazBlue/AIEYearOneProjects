#ifndef GAME_H
#define GAME_H

#include <string>

#include "Graph.h"


using namespace std;

class SpriteBatch;
class PathFinder;
class Agent;
//class Graph;
//class Node;

class Game
{
public:
	~Game();
	Game(string a_gameName, unsigned int a_width, unsigned int a_height);

	void Run();

	friend class SpriteBatch;

	enum PathUpdate
	{
		PATHUPDATE_BYFRAME,
		PATHUPDATE_INSTANT,
		PATHUPDATE_MANUAL
	};

	PathUpdate updateInterval;

private:

	void Load();

	void GenRndmGraph();
	//loop through all nodes and add edges as needed
	void ConEdges();

	void GhostUpdate();

	void Draw();
	void Update(float a_dt);

	string m_title;

	unsigned int m_width, m_height;

	SpriteBatch* m_spriteBatch;

	//graph containing nodes and pathfinding
	Graph* m_Graph;

	PathFinder* m_pathFinder;

	//single agent for testing
	list<Agent*> m_agents;

	//pointers to active nodes for colouring
	Graph::Node* m_hoverNode, *m_startNode, *m_endNode, *m_ghostNode;


	//tracking time
	float m_deltaTime, m_lastTime, m_currentTime;

	//tracking the mouse pos
	Vec2 m_mPos;

	//main game window - set in spritebatch init
//	GLFWwindow* m_window;

	float m_lastClick, m_clickTime;
};


#endif