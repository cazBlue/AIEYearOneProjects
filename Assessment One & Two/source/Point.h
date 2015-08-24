#ifndef POINT_H
#define POINT_H

#include <iostream>

//TODO
/*

Change Log
declare the deconstructor - done 17/7/2014
*/

using namespace std;

//declare the point class
//the point class is a custom xyz
class Point
{
public:
	//declare the default and default overload constructor
	Point(float f_x = 1.0, float f_y = 1.0, float f_z = 1.0);

	//declare the destructor, called as the class is being destroyed
	~Point();

	void getXYZ(float &X, float &Y, float &Z);
	
	//single get functions
	float getX();
	float getY();
	float getZ();

	void setXYZ(float X, float Y, float Z);	
	//void setXYZ(float Arg);	//overloaded function

	//single setter funcitons
	void setX(float X);
	void setY(float Y);
	void setZ(float Z);

	//declare function to override the = operator for this class
	Point operator = (Point &p);

private:
	//declare the xyz variables for use, these are private to avoid direct access
	float x, y, z;

protected:

};

//declare functions to modify the + and - operator to handle the Point class
ostream &operator <<(ostream &stream, Point &p);
istream &operator >>(istream &stream, Point &p);

#endif