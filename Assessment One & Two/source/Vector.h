#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"


//vector class inherits from point class
class Vector : public Point
{
public:
	Vector(float X=0, float Y=0, float Z=0);

	//define operator overrides for + and -
	Vector operator +(Vector &p);
	Vector operator -(Vector &p);
};

#endif