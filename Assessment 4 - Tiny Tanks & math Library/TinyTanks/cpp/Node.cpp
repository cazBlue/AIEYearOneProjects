#include "Node.h"

Node::Node(Mat3 a_transform)
{
	m_transform_Mat3 = Mat3::CreateIdentity() * a_transform;
}

Mat3 Node::getFinalTransform()
{
	//multiply agaisnt a parent node if it exists
	//recusivly go up the chain to ensure actual position
	//through this function
	if(m_ptr_parentNode_Mat3)
		return  m_transform_Mat3 * m_ptr_parentNode_Mat3->getFinalTransform();

	return  m_transform_Mat3;
}

void Node::setParentNode(Node *a_parentNode)
{
	m_ptr_parentNode_Mat3 = a_parentNode;
}

Mat3 Node::getNodeTransform()
{
	return m_transform_Mat3;
}

Mat3* Node::manipMat3()
{
	return &m_transform_Mat3;
}