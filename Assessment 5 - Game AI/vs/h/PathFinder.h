#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "Graph.h"

#include <list>
#include <vector>

using namespace std;

class PathFinder
{
public:

	/*
		path finder class is used to find a path through the graph

	*/


	class Node
	{
	public:

		Node();
		Node(Graph::Node* a_node);
		~Node();

		//pointer to a node in the main graph
		Graph::Node* m_node;

		//pointer to this nodes parent if it has one
		PathFinder::Node* m_parent;

		//degress of seperation 
		int m_dos;

		float m_gScore, m_hScore, m_fScore;
	};

	enum PATH_TYPE
    {
        PATH_TYPE_BREDTH_FIRST_SEARCH,
        PATH_TYPE_DIJKSTRAS,
        PATH_TYPE_ASTAR,
    };


	//path finding with known end
	//ASTAR requires a known end
	//if astar chosen with no known end Dijkstras will be used instead
	void BeginPathFinding(Graph::Node* a_start, Graph::Node* a_end, PATH_TYPE a_pathType);
	
	//path finding for when an end is not known
	//Djikstras/depth-breadth
	void BeginPathFinding(Graph::Node* a_start, list<Graph::Node*> a_potintial_ends, PATH_TYPE a_pathType);
	
	bool CheckEnd(Graph::Node* a_node);
//	bool CheckEnd(list<Graph::Node*> a_nodes, Graph::Node* a_endNodeType);
	void Reset();

	bool IsPathFound();

	bool RequiresUpdate();

	void Update();

	void ProcessNode(Node* parent);


	list<Node*> m_OpenList;
	list<Node*> m_ClosedList;


	vector<Graph::Node*>m_Path;
	//list<Graph::Node*> m_Path;

	//set to Dijkstras
	PATH_TYPE m_pathFindType;

	//TODO look into c++ delegates vs c# delegates
	bool m_endCondition;

	Graph::Node* m_endNode, *m_startNode;
	list<Graph::Node*> m_potEnds;

	PathFinder();



private:


};




#endif