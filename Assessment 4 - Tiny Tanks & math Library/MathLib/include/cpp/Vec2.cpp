#include "Vec2.h"
//#include <iostream>
//#define _USE_MATH_DEFINES
#define M_PI       3.14159265358979323846
#include <math.h>
#include "Mat3.h"

using namespace std;

//default constructor
Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float a_x, float a_y)
{
	x = a_x;
	y = a_y;
}

Vec2::Vec2(float * a_fptr)
{
	x = a_fptr[0];
	y = a_fptr[1];
}


Vec2& Vec2::operator += (const Vec2 &rhs )
{
	/*
	x,y are member variables of the left hand operator (foo)
	bar is self or any Vec2 object
	foo += bar;
	*/
	y = y + rhs.y;
	x = x + rhs.x;
	return *this;
}

Vec2& Vec2::operator -= (const Vec2 &rhs)
{
	y = y - rhs.y;
	x = x - rhs.x;
	return *this;
}

Vec2& Vec2::operator *= (const Vec2 &rhs)
{
	y = y * rhs.y;
	x = x * rhs.x;
	return *this;
}

Vec2& Vec2::operator /= (const Vec2 &rhs)
{
	y = y / rhs.y;
	x = x / rhs.x;
	return *this;
}

Vec2& Vec2::operator += (float rhs)
{
	y = y + rhs;
	x = x + rhs;
	return *this;
}

Vec2& Vec2::operator -= (float rhs)
{
	y = y - rhs;
	x = x - rhs;
	return *this;
}

Vec2& Vec2::operator /= (float rhs)
{
	y = y / rhs;
	x = x / rhs;
	return *this;
}

Vec2 Vec2::operator * (const Mat3 &rhs) const
{
	/*
	vector2 * comlum 1 and 2 of mat3
		x = x  *	m0
					m3
					m6

		y = y  *	m1
					m4
					m7
	*/
	//return Vec2( (rhs.getMatElement(0) * x) + (rhs.getMatElement(3) * y) + rhs.getMatElement(6), 
	//			 (rhs.getMatElement(1) * x) + (rhs.getMatElement(4) * y) + rhs.getMatElement(7) );
	return Vec2((x*rhs.m1)+(y*rhs.m4) + rhs.m7,(x*rhs.m2)+(y*rhs.m5)+ rhs.m8);
}

Vec2 Vec2::operator + ( const Vec2 &rhs ) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - ( const Vec2 &rhs ) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * ( const Vec2 &rhs ) const
{
	return Vec2(x * rhs.x, y * rhs.y);
}

Vec2 Vec2::operator / ( const Vec2 &rhs ) const
{
	return Vec2(x / rhs.x, y / rhs.y);
}

Vec2 Vec2::operator + ( float rhs ) const
{
	return Vec2(x + rhs, y + rhs);
}

Vec2 Vec2::operator - ( float rhs ) const
{
	return Vec2(x - rhs, y - rhs);
}

Vec2 Vec2::operator * ( float rhs ) const
{
	return Vec2(x * rhs, y * rhs);
}

Vec2 Vec2::operator / ( float rhs ) const
{
	return Vec2(x / rhs, y / rhs);
}

float Vec2::Length() const
{
	return sqrt((x * x) + ( y * y ));
}

Vec2 Vec2::GetNormalised() const
{
	float len = Length();
	if(len > 0)
		return Vec2((x / len), (y / len));
	else
		return Vec2(0,0);
}

/*
===============Static functions
*/
float Vec2::Length(const Vec2 VectoGet) 
{
	return sqrt((VectoGet.x * VectoGet.x) + ( VectoGet.y * VectoGet.y ));
}

Vec2 Vec2::GetNormalised(const Vec2 VecToGet) 
{
	float len = VecToGet.Length();
	return Vec2((VecToGet.x / len), (VecToGet.y / len));
}

float Vec2::Dot( const Vec2 &lhs, const Vec2 &rhs )
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

float radToDeg(float rad)
{
	return (float)(rad * (180.0 / M_PI ));	
}

//------------------------------------

float Vec2::Dot(const Vec2 &rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}

float Vec2::GetAngleBetweenTwo( const Vec2 &VecOne, const Vec2 &VecTwo)
{
	Vec2 AP1, AP2;
	AP1.x = VecOne.y;
	AP1.y = -VecOne.x;

	//AP2.x = VecTwo.y;
	//AP2.y = -VecTwo.x;

	float rot = acosf(AP1.Dot(VecTwo));

	if(AP1.Dot(VecTwo) < 0)
		rot = -rot;
	
	//if(radOrDeg)
	//	return (float)(rot * (180.0 / M_PI ));	
	//else
		return rot;
}

float Vec2::GetAngleBetween( const Vec2 &rhs, bool radOrDeg) const
{		
	Vec2 r = rhs;
	r.Normalise();

	Vec2 A = GetNormalised();
	Vec2 AP(A.y, -A.x);


	float rot = acosf(this->Dot(r));
	if(AP.Dot(r) < 0)
		rot = -rot;
	
	if(radOrDeg)
		return (float)(rot * (180.0 / M_PI ));	
	else
		return rot;
}

Vec2 Vec2::GetPerpendicularCW() const
{
	return Vec2(y,x);
}

Vec2 Vec2::GetPerpendicularCCW() const
{
	return Vec2( -y, x );
}

Vec2& Vec2::Normalise()
{
	return *this = GetNormalised();
}
