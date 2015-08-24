#include "Vector.h"

Vector::Vector(float X, float Y, float Z) : Point(X, Y, Z)
{
	//inherits functions from Point class	
}

//custon + operator for the vector class
Vector Vector::operator +(Vector &p)
{
	Vector outV;
	outV.setX(getX() + p.getX());
	outV.setY(getY() + p.getY());
	outV.setZ(getZ() + p.getZ());

	return outV;
}

//custom - operator for the vector class
Vector Vector::operator -(Vector &p)
{
	Vector outV;
	outV.setX(getX() - p.getX());
	outV.setY(getY() - p.getY());
	outV.setZ(getZ() - p.getZ());

	return outV;
}