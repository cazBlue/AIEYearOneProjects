// Author: Created by Aaron Cox
//UPdated by Callan Winfield
// Description: basic vertex 2 class...

#ifndef VEC2_H
#define VEC2_H

class Mat3;

class Vec2
{
public:

	float x;
	float y;

public:
	/**
	default constructor
	*/
	Vec2();

	///constructor defining a default position
	Vec2(float X, float Y);
	///ptr of float array float[x], float[y]
	Vec2(float *ptr);

	///operator overload for custom Vec2 += Vec2
	Vec2& operator += (const Vec2 &rhs );
	///operator overload for custom Vec2 -= Vec2
	Vec2& operator -= ( const Vec2 &rhs );
	///operator overload for custom Vec2 *= Vec2
	Vec2& operator *= ( const Vec2 &rhs );
	///operator overload for custom Vec2 /= Vec2
	Vec2& operator /= ( const Vec2 &rhs );

	///operator overload for custom Vec2 += float
	Vec2& operator += ( float rhs );
	///operator overload for custom Vec2 -= float
	Vec2& operator -= ( float rhs );
	///operator overload for custom Vec2 *= float
	Vec2& operator *= ( float rhs );
	///operator overload for custom Vec2 /= Vec2
	Vec2& operator /= ( float rhs );
	
	///operator overload for custom Vec2 + Mat3
	Vec2 operator * (const Mat3 &rhs) 			const;

	///operator overload for custom Vec2 + Vec2
	Vec2 operator + ( const Vec2 &rhs )			const;
	///operator overload for custom Vec2 - Vec2
	Vec2 operator - ( const Vec2 &rhs )			const;
	///operator overload for custom Vec2 * Vec2
	Vec2 operator * ( const Vec2 &rhs )			const;
	///operator overload for custom Vec2 / Vec2
	Vec2 operator / ( const Vec2 &rhs )			const;
	
	///operator overload for custom Vec2 + float
	Vec2 operator + ( float rhs )				const;
	///operator overload for custom Vec2 - float
	Vec2 operator - ( float rhs )				const;
	///operator overload for custom Vec2 * float
	Vec2 operator * ( float rhs )				const;
	///operator overload for custom Vec2 / float
	Vec2 operator / ( float rhs )				const;

	///returns length of this Vec2 from 0,0
	///length is the same as magnitude
	float	Length()							const;	
	///return the normalised Vec2
	///value from 0,0 to 1,1
	Vec2	GetNormalised()						const;
	///return the dot product, the length of the overlay of two Vec2's
	//compares this to passed Vec2
	float	Dot( const Vec2 &rhs )				const;

	///return the normalised Vec2 of the passed Vec2
	static Vec2	GetNormalised(const Vec2 VecToGet);
	///returns length of passed Vec2 from 0,0
	///length is the same as magnitude
	static float Length(const Vec2 VecToGet);
	///return the dot product of two passed Vec2, the length of the overlay of two Vec2's
	static float Dot( const Vec2 &lhs, const Vec2 &rhs );

	///returns radian or degree on bool value
	///returns radians between two angles, this and passed Vec2
	float GetAngleBetween( const Vec2 &rhs, bool radOrDeg = false)	const;

	///returns radians between angles, using passed Vec2's
	static float GetAngleBetweenTwo( const Vec2 &vecOne, const Vec2 &vec2Two);

	///returns perpendicular Vec2 clockwise
	Vec2 GetPerpendicularCW()					const;
	///returns perpendicular Vec2 counter clockwise
	Vec2 GetPerpendicularCCW()					const;

	/// normalises the vector and returns *this 
	Vec2& Normalise(); 
	
	///convert Rad to degrees
	static float radToDeg(float rad);

};

#endif VEC2_H