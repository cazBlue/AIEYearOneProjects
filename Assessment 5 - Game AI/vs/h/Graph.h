#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "Vec2.h"


//class Vec2;

using namespace std;


class Graph
{
public:
	
	//pre-declare Edge class
	class Edge;

	class Node
	{
	public:
		list<Edge* > m_edges;

		Vec2 m_pos;
		Node();
		~Node();
		Node(Vec2 a_pos);

		unsigned int m_ID;
				
		unsigned static int ms_nextID;
	};

	class Edge
	{
	public:
		Node* m_connection;
		float m_cost;
		//Edge();
		Edge(Node* a_con, float a_cost);
	};
	
	//default graph constructor
	Graph();

	//create a new node
	//returns created node
	//node will be added to the graph node list m_nodes
	Node* AddNode(Vec2 a_pos);

	//add an edge connection FROM node1 TO node2
	//the cost to traverse from node1 to node2 for path finding
	void AddConnection(Node* a_node1, Node* a_node2, float a_cost);
	
	//pos is th centre of the node
	//will return a list of nodes within range/radius
	list<Node*> FindNodesInRange(Vec2 a_pos, float a_range);

	Node* GetNode(int a_id);

	friend class Game;

private:
	//all nodes in graph
	list<Node*> m_nodes;
};




#endif