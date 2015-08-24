#include "Graph.h"
//#include "Vec2.h"


//***************** NODE

//init the static node counter
unsigned int Graph::Node::ms_nextID = 0;

Graph::Node::~Node()
{
}

Graph::Node::Node()
{
}

Graph::Node::Node(Vec2 a_pos)
{
	m_pos = a_pos;
	m_ID = Graph::Node::ms_nextID;
	Graph::Node::ms_nextID += 1;
}


//***************** GRAPH

Graph::Graph()
{
	//create a list to hold active nodes
	list<Node> Nodes;

}


Graph::Node* Graph::AddNode(Vec2 a_pos)
{
	Node* myNode = new Node(a_pos);
	m_nodes.push_back(myNode);

	return myNode;
}


void Graph::AddConnection(Node* a_node1, Node* a_node2, float a_cost)
{
	//check if nodes exist
	if(a_node1 == nullptr || a_node2 == nullptr)
		return;

	Edge* myEdge = new Edge(a_node2, a_cost);

	a_node1->m_edges.push_back(myEdge);
}


list<Graph::Node*> Graph::FindNodesInRange(Vec2 a_pos, float a_range)
{
	list<Node*> nodeList;

	list<Node*>::iterator iter;
	for(iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
	{
		//compare the iternode against the position of this node
		//and the desired search range
		Vec2 iPos = (*iter)->m_pos;

		if((iPos - a_pos).Length() < a_range)
		{
			nodeList.push_back((*iter));
		}
	}
	return nodeList;
}


Graph::Node* Graph::GetNode(int a_id)
{
	//loop through all current nodes and check id's
	list<Node*>::iterator iter;
	for(iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
	{
		if((*iter)->m_ID == a_id)
		{
			return (*iter);
		}
	}

	return nullptr;
}


//***************** EDGE

Graph::Edge::Edge(Node* a_con, float a_cost)
{
	if(a_con != nullptr)
		m_connection = a_con;
	else
		m_connection = nullptr;

	m_cost = a_cost;
}