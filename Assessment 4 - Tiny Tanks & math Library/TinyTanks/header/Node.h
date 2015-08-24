#ifndef NODE_H
#define NODE_H

#include "Mat3.h"

class Vec2;

class Node
{
public:
	//default constructor, this node will have no parent
	Node(Mat3 a_transform);
	//create node and set parent node
	Node(Mat3 a_transform, Node *a_parentNode);
	
	//returns transform of parent transform
	Mat3 getParentTransform();
	
	//return the final transform, should * the child/parent
	Mat3 getFinalTransform();

	//get this transform, as in just this node without taking
	//the parent into account even if one exists
	Mat3 getNodeTransform();

	//get the pointer to the node for updating transforms
	Mat3* manipMat3();

	//get a pointer to the parent node
	Node* getParentNode();

	//set the parent node to a differnt node
	void setParentNode(Node * a_parentNode);

	
	//detach parent node
	void removeParnetNode();

private:
	//pointer to this nodes parent
	Node *m_ptr_parentNode_Mat3;

	//transform of this node
	Mat3 m_transform_Mat3;
};








#endif