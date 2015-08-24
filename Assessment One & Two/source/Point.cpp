#include <iostream>
#include "Point.h"

using namespace std;


//initial constructor
Point::Point(float f_x, float f_y, float f_z)
{
	//cout << "we are in the constructor with args" << endl;

	x = f_x;
	y = f_y;
	z = f_z;
}
//destructor for class
Point::~Point()
{
	cout << "this is destroyed" << endl;
}

//-----------------------------
//------------single get functions
//-----------------------------
float Point::getX()
{
	return x;
}
float Point::getY()
{
	return y;
}
float Point::getZ()
{
	return z;
}

//-----------------------------
//------------single set functions
//-----------------------------

void Point::setX(float X)
{
	x = X;
}
void Point::setY(float Y)
{
	y = Y;
}
void Point::setZ(float Z)
{
	z = Z;
}

/*
//example of overloaded function
void setXYZ(float Arg)
{
	cout << "overload function" << endl;
}
*/

//setter for xyz
//builds on single functions
void Point::setXYZ(float X, float Y, float Z)
{
	setX(X);
	setY(Y);
	setZ(Z);
}

//getter for xyz
//builds on single functions
void Point::getXYZ(float &X, float &Y, float &Z)
{
	X = getX();
	Y = getY();
	Z = getZ();
}

//custom = operator for this class
//sets the current value of the of THIS object to the value of the referenced object (not that this is passing in reference, allows calling of functions on the object itself)
Point Point::operator = (Point &p)
{
	setX(p.getX());
	setY(p.getY());
	setZ(p.getZ());

	return *this;
}

//update << operator to work correctly with output
//is taking in the ostram object and a reference to the current point object
ostream &operator <<(ostream &stream, Point &p)
{
	stream << p.getX() << " " << p.getY() << " " << p.getZ();
	return stream;
}

//update >> operator to work correctly with input
//is taking in the ostram object and a reference to the current point object
istream &operator >>(istream &stream, Point &p)
{
	float x, y, z;

	stream >> x >> y >> z;
	p.setXYZ(x,y,z);	
	return stream;
}

//------------------------------------------------
//Point class end
//------------------------------------------------