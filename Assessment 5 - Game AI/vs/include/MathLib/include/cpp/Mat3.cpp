#include "Mat3.h"
#include "Vec2.h"
#include <math.h>
#include <memory>

Mat3::Mat3()
{
	//give this a default identity
	*this = CreateIdentity();
}


Mat3::Mat3(	float a1, float a2, float a3,  
			float a4, float a5, float a6,
			float a7, float a8, float a9 )
{
	/*
	m1 = a1;
	m2 = a2;
	m3 = a3;
	m4 = a4;
	m5 = a5;
	m6 = a6;
	m7 = a7;
	m8 = a8;
	m9 = a9;
	*/

	matElement[0] = a1;
	matElement[1] = a2;
	matElement[2] = a3;
	matElement[3] = a4;
	matElement[4] = a5;
	matElement[5] = a6;
	matElement[6] = a7;
	matElement[7] = a8;
	matElement[8] = a9;

	// THIS IS BAD! BUT AWESONE
	// from aaron
	//memcpy(this, &a1, sizeof(float) * 9);

}

Mat3::Mat3(	float *ptr )
{
	matElement[0] = ptr[0];
	matElement[1] = ptr[1];
	matElement[2] = ptr[2];
	matElement[3] = ptr[3];
	matElement[4] = ptr[4];
	matElement[5] = ptr[5];
	matElement[6] = ptr[6];
	matElement[7] = ptr[7];
	matElement[8] = ptr[8];
}


float Mat3::getMatElement(int aElement) const
{
	return matElement[aElement];
}

void Mat3::setMatElement(int el, float val)
{
	matElement[el] = val;
}

Mat3 Mat3::CreateIdentity()
{
	return Mat3(1, 0, 0, 
				0, 1, 0,
				0, 0, 1);
	/* array reference
				0,1,2	
				3,4,5
				6,7,8	
	*/
}

Mat3 Mat3::CreateTranslation(float x, float y)
{
	Mat3 matrix = Mat3::CreateIdentity();
	matrix.setMatElement(6 , x);
	matrix.setMatElement(7 , y);
	return matrix;

	// return Mat3(1, 0, 0,
	// 			0, 1, 0,
	// 			x, y, 1);
}

Mat3 Mat3::CreateTranslation(Vec2 a_pos)
{
	return CreateTranslation(a_pos.x, a_pos.y);
}

Mat3 Mat3::CreateScale(float yScale, float xScale)
{
	Mat3 matrix = Mat3::CreateIdentity();
	matrix.setMatElement(0 , xScale);
	matrix.setMatElement(4 , yScale);

	return matrix;
}

Mat3 Mat3::operator*(const Mat3 &rhs) const
{
	/* array reference
				0,1,2	
				3,4,5
				6,7,8	
	*/
	//M1(this) * M2(rhs)
	Mat3 matrix;

	//----------------------------row one
	//row one by colum one
	matrix.setMatElement(0, 
						(getMatElement(0) * rhs.getMatElement(0)) + 
						(getMatElement(1) * rhs.getMatElement(3)) +
						(getMatElement(2) * rhs.getMatElement(6)));
	//row one by colum two
	matrix.setMatElement(1, 
						(getMatElement(0) * rhs.getMatElement(1)) + 
						(getMatElement(1) * rhs.getMatElement(4)) +
						(getMatElement(2) * rhs.getMatElement(7)));
	//row one by colum three
	matrix.setMatElement(2, 
						(getMatElement(0) * rhs.getMatElement(2)) + 
						(getMatElement(1) * rhs.getMatElement(5)) +
						(getMatElement(2) * rhs.getMatElement(8)));
	//----------------------------row two
	//row two by colum one
	matrix.setMatElement(3, 
						(getMatElement(3) * rhs.getMatElement(0)) + 
						(getMatElement(4) * rhs.getMatElement(3)) +
						(getMatElement(5) * rhs.getMatElement(6)));
	//row two by colum two
	matrix.setMatElement(4, 
						(getMatElement(3) * rhs.getMatElement(1)) + 
						(getMatElement(4) * rhs.getMatElement(4)) +
						(getMatElement(5) * rhs.getMatElement(7)));
	//row two by colum three
	matrix.setMatElement(5, 
						(getMatElement(3) * rhs.getMatElement(2)) + 
						(getMatElement(4) * rhs.getMatElement(5)) +
						(getMatElement(5) * rhs.getMatElement(8)));
	//----------------------------row three
	//row three by colum one
	matrix.setMatElement(6, 
						(getMatElement(6) * rhs.getMatElement(0)) + 
						(getMatElement(7) * rhs.getMatElement(3)) +
						(getMatElement(8) * rhs.getMatElement(6)));
	//row three by colum two
	matrix.setMatElement(7, 
						(getMatElement(6) * rhs.getMatElement(1)) + 
						(getMatElement(7) * rhs.getMatElement(4)) +
						(getMatElement(8) * rhs.getMatElement(7)));
	//row three by colum three
	matrix.setMatElement(8, 
						(getMatElement(6) * rhs.getMatElement(2)) + 
						(getMatElement(7) * rhs.getMatElement(5)) +
						(getMatElement(8) * rhs.getMatElement(8)));
	return matrix;
}


Mat3& Mat3::operator *=(const Mat3 &rhs)
{
	return *this = *this * rhs;
}

void Mat3::SetTranslation(float x, float y)
{
	setMatElement(6, x);
	setMatElement(7, y);
}

void Mat3::SetTranslation(Vec2 vec)
{
	SetTranslation(vec.x, vec.y);
}

void Mat3::Translate(float x, float y)
{	
	SetTranslation(getMatElement(6) + x , getMatElement(7) + y);
}

void Mat3::Translate(Vec2 vec)
{
	Translate(vec.x, vec.y);
}

void Mat3::SetRotation(float rad)
{
	/*
		cos(a), -sin(a), 0
		sin(a), cos(a), 0
		0	,	0	,	1
	*/	
	SetUpRotation(rad);
	SetRightRotation(rad);
}

void Mat3::SetUpRotation(float rad)
{
	float scale = GetUpScale();

	setMatElement(3 , sin(rad) * scale);
	setMatElement(4 , cos(rad) * scale);
}

void Mat3::SetRightRotation(float rad)
{
	float scale = GetRightScale();

	setMatElement(0 ,  cos(rad) * scale);
	setMatElement(1 , -sin(rad) * scale);
}

void Mat3::Rotate(float rot)
{
	/*
	setMatElement(0, cos(getMatElement(0) + rad));
	setMatElement(1, (sin(getMatElement(1) + rad)) * -1);
	setMatElement(3, sin(getMatElement(3) + rad));
	setMatElement(4, cos(getMatElement(4) + rad));
	*/
	//Mat3 matrix = Mat3::CreateIdentity() * CreateRotation(rad);
	
	//*this = *this * matrix;
	Vec2 scale = GetScale();
	float upRotation = GetUpRot();
	float rightRotation = GetRightRot();

	m1 = (cos(rot+rightRotation)) * scale.x; m2 = (-sin(rot+rightRotation)) * scale.x; 
	m4 = (sin(rot+upRotation))    * scale.y; m5 = (cos(rot+upRotation))     * scale.y; 
}

Mat3 Mat3::CreateRotation(float rot)
{
	/*
		cos(a), -sin(a), 0
		sin(a), cos(a), 0
		0	,	0	,	1
	*/
	Mat3 matrix = Mat3::CreateIdentity();
	matrix.setMatElement(0 , cos(rot));
	matrix.setMatElement(1 , -sin(rot));
	matrix.setMatElement(3 , sin(rot));
	matrix.setMatElement(4 , cos(rot));
	return matrix;
}


float Mat3::GetUpRot() const
{
	Vec2 up(0, 1);
	Vec2 upMatVec(m4, m5);

	upMatVec.Normalise();
	return up.GetAngleBetween(upMatVec);
}

float Mat3::GetRightRot() const
{
	Vec2 right(1, 0);
	Vec2 rightMatVec(m1, m2);

	rightMatVec.Normalise();
	return right.GetAngleBetween(rightMatVec);
}

/*
float Mat3::GetUpRot() const
{	
	return atan2(getMatElement(3), getMatElement(4));
}

float Mat3::GetRightRot() const
{	
	return atan2(getMatElement(0), getMatElement(1));
}
*/

void Mat3::SetScale(float x, float y)
{
	/* array reference
			0,1,2  m1, m2, m3	
			3,4,5  m4, m5, m6
			6,7,8	
	*/

	float xScale = GetRightScale();
	float yScale = GetUpScale();

	m1 = (m1 / xScale) * x;
	m2 = (m2 / xScale) * x;
	m4 = (m4 / yScale) * y;
	m5 = (m5 / yScale) * y;

	//Mat3 matrix = Mat3::CreateIdentity() * Mat3::CreateScale(x,y);
	//*this = *this * matrix;
}

float Mat3::GetUpScale() const
{
	Vec2 yAxis(m4, m5);
	//Vec2 yAxis(getMatElement(3), getMatElement(4));
	return yAxis.Length();
	//return Vec2(m4, m5).Length();
	//return getMatElement(4);
}

float Mat3::GetRightScale() const
{
	//Vec2 xAxis(getMatElement(0), getMatElement(1));
	Vec2 xAxis(m1, m2);
	return xAxis.Length();
	//return Vec2(m1, m2).Length();
	//return getMatElement(0);
}

Vec2 Mat3::GetScale() const
{
	return Vec2(GetRightScale(), GetUpScale());
}


Vec2 Mat3::TransformPoint(Vec2 const &vec ) const
{
	return vec * *this;
	//return Vec2(getMatElement(6) * vec.x, getMatElement(7) * vec.y);
}


Vec2 Mat3::GetTranslation() const
{
	/* array reference
				0,1,2	
				3,4,5
				6,7,8	
	*/
	return Vec2(getMatElement(6), getMatElement(7));
}

Vec2 Mat3::getUpVec() const
{
	return Vec2(m4, m5);
}