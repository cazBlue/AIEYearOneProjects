#include "PathFinder.h"

#include <cmath>

#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <algorithm>
//=========================== Node class

PathFinder::Node::~Node()
{

}


PathFinder::Node::Node(Graph::Node* a_node)
{
	m_node = a_node;
	m_dos = 0;
	m_gScore = 0;
	m_fScore = 0;
	m_hScore = 0;
}


PathFinder::Node::Node()
{
	m_dos = 0;
	m_gScore = 0;
	m_fScore = 0;
	m_hScore = 0;
	m_node = nullptr;
}

//============================== Path finder class

PathFinder::PathFinder()
{
	m_pathFindType = PATH_TYPE_DIJKSTRAS;
}


void PathFinder::BeginPathFinding(Graph::Node* a_start, Graph::Node* a_end, PATH_TYPE a_pathType)
{
	Reset();

	//push the start node onto the list for processing, the is the first processed node
	m_OpenList.push_back(new PathFinder::Node(a_start));

	//set pointer to the start/end node
	m_endNode = a_end;
	m_startNode = a_start;

	//set the path finding type, works with anything as the ending node is known
	m_pathFindType = a_pathType;
}

void PathFinder::BeginPathFinding(Graph::Node* a_start, list<Graph::Node*> a_potintial_ends, PATH_TYPE a_pathType)
{
	Reset(); //reset path finding vars for new run

	//push the start node onto the list for processing, the is the first processed node
	m_OpenList.push_back(new PathFinder::Node(a_start));

	//set pointer to the start node
	m_startNode = a_start;

	//as we have potential ends Astar cannot be used, revert to Dijkstras
	m_pathFindType = a_pathType;
	if(m_pathFindType == PATH_TYPE_ASTAR)
		m_pathFindType = PATH_TYPE_DIJKSTRAS;
}

void PathFinder::Reset()
{
	m_Path.clear();
	m_OpenList.clear();
	m_ClosedList.clear();

	m_endCondition = false;

	m_endNode = nullptr;
	m_startNode = nullptr;
}



bool PathFinder::CheckEnd(Graph::Node* a_node)
{
	//if we are checking a list
	if(m_potEnds.size() > 0)
	{
		list<Graph::Node*>::iterator iter;
		for(iter = m_potEnds.begin(); iter != m_potEnds.end(); iter++)
		{
			if((*iter) == a_node)
				return true;
		}
		return false;
	}

	//if we are checking a single node
	return m_endNode == a_node;
}


bool PathFinder::RequiresUpdate()
{
	//check if a path exists, if not keep going
	if(IsPathFound())
		return false;

	//check if anything is on the open list, if so 
	//keep going while searching for path
	if(m_OpenList.size() == 0)
		return false;

	//nothing to do
	return true;
}

bool PathFinder::IsPathFound()
{
	return m_Path.size() > 0;
}


void PathFinder::Update()
{
	//=============== early exits - no need to process nodes if these are true

	//check if we have found the end
	if (m_endCondition)
		return;
	
	//check if a path has been found
	if (IsPathFound())
		return;

	if (m_OpenList.size() == 0)
		return;

	//============== end early exits

	//get a node of the open list, for the first run this will be the start node
	PathFinder::Node* node = m_OpenList.front();	
	m_OpenList.remove(m_OpenList.front());

	m_ClosedList.push_back(node);

	//if we have found an end node build the path	
	//the path will be found in the current "update" - this means that the current node is the end node of the path
	if(CheckEnd(node->m_node))
	{
		//get the last added node and track back buildng
		PathFinder::Node* current = node;

		while(current->m_node != m_startNode)
		{
			m_Path.push_back(current->m_node);
			
			current = current->m_parent;
		}
		m_Path.push_back(m_startNode);
	}

	//procerss the current node
	//assigns weigting and pathfinding scores to a given node
	ProcessNode(node);

	//sort the open list based on selected path finding algorithym
	//the list is sorted as each node is updated
	//this is the "priority que"

	//sort is using lambdas with < sorting
	switch (m_pathFindType)
	{
	case PathFinder::PATH_TYPE_BREDTH_FIRST_SEARCH:
		m_OpenList.sort([](PathFinder::Node * lhs, PathFinder::Node * rhs){return lhs->m_dos < rhs->m_dos;});
		break;
	case PathFinder::PATH_TYPE_DIJKSTRAS:
		m_OpenList.sort([](PathFinder::Node * lhs, PathFinder::Node * rhs){return lhs->m_gScore < rhs->m_gScore;});
		break;
	case PathFinder::PATH_TYPE_ASTAR:
		m_OpenList.sort([](PathFinder::Node * lhs, PathFinder::Node * rhs){return lhs->m_fScore < rhs->m_fScore;});
		break;
	default:
		break;
	}
}


void PathFinder::ProcessNode(PathFinder::Node* parent)
{
	list<Graph::Edge*>::iterator edgeIter;

	//for every edge in the current node
	//pick one to add to the open and closed list, or if an end is found 
	//process the shortest path
	for(edgeIter = parent->m_node->m_edges.begin(); 
		edgeIter != parent->m_node->m_edges.end(); 
		edgeIter++)
	{
		Graph::Node* node = (*edgeIter)->m_connection;

		//----------------------------------------------
		//calculate the G, H and F score for the node
		//----------------------------------------------
		//adds a degree of seperation to this node.
		int dos = parent->m_dos + 1;
		//init the Heuristic
		float h = 0;



		if(m_endNode != nullptr)
		{
			//get the length from the current node to the end node
			float len = (m_endNode->m_pos - node->m_pos).Length();
			//set the Heuristic to the length ^ 2 for tighter and more accurate results
			h = pow(len,2);

			//h = len;

			//h *= 3;
		}

		//set the g score to the parents g score + the edge cost for this edge
		float g = parent->m_gScore + (*edgeIter)->m_cost;
		float f = g + h;
		//----------------------------------------------

		bool canAddNode = true;

		//check if the node is in the open list?
		//if it is mark it for not adding and set the nodes scores
		list<PathFinder::Node*>::iterator iterOpen;
		for(iterOpen = m_OpenList.begin(); iterOpen != m_OpenList.end(); iterOpen++)
		{
			if((*iterOpen)->m_node == node)
			{
				//if it's faster getting to this node from this parent update the node
				if(g < (*iterOpen)->m_gScore)
				{
					(*iterOpen)->m_parent = parent;
					(*iterOpen)->m_gScore = g;
					(*iterOpen)->m_hScore = h;
					(*iterOpen)->m_fScore = f;
					(*iterOpen)->m_dos = dos;
				}

				canAddNode = false;
				break;
			}
		}
		
		list<PathFinder::Node*>::iterator iterClosed;
		for(iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed++)
		{
			if((*iterClosed)->m_node == node)
			{
				//if it's faster getting to this node from this parent update the node
				if(g < (*iterClosed)->m_gScore)
				{
					(*iterClosed)->m_parent = parent;
					(*iterClosed)->m_gScore = g;
					(*iterClosed)->m_hScore = h;
					(*iterClosed)->m_fScore = f;
					(*iterClosed)->m_dos = dos;
				}
                canAddNode = false;
                break;
			}
		}

		if(canAddNode)
		{
			PathFinder::Node* newNode = new PathFinder::Node(node);

			newNode->m_parent = parent;
			newNode->m_gScore = g;
			newNode->m_hScore = h;
			newNode->m_fScore = f;
			newNode->m_dos = dos;

			m_OpenList.push_back(newNode);
		}
	}
}