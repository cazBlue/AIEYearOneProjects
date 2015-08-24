#ifndef MAT3_H
#define MAT3_H

#include "Vec2.h"

//pre-define to avoid cyclic error
//class Vec2;

class Mat3
{
private:
		
	union
	{
		struct
		{
			float 	m1, m2, m3,
					m4, m5, m6,
					m7, m8, m9;
		};

		//TODO ask about the use of the below in comparison to the use of the above
		struct
		{
			float matElement[9];
		};

		struct
		{
			float mm[3][3];
		};

	};

public:
	
	// CONSTRUCTORS
	//-------------------------------------------------------------------------
		/// default to the identity matrix
		Mat3();
		
		/// ptr is a pointer to the beginning of an array of 9 floats
		Mat3(float *ptr); 
		
		//manually set members
		Mat3(	float m1, float m2, float m3,  
				float m4, float m5, float m6,
				float m7, float m8, float m9 );
	
	// Helper Creational matrix functions...
	// useage: Mat3 myMat1 = Mat3::CreateRotation(45) * Mat3::CreateTranslation(10, 10);
	//-------------------------------------------------------------------------
		//create blank identity matrix
		static Mat3 CreateIdentity();
		/// create translation only for x,y - does not have scale/rotation info
		static Mat3 CreateTranslation(float x, float y);
		/// create translation matrix only for Vec2(x,y) - does not have scale/rotation info
		static Mat3 CreateTranslation(Vec2 a_pos);
		///create rotation matrix
		static Mat3 CreateRotation(float rot);
		///create scale matrix
		static Mat3 CreateScale( float yScale, float xScale);
	
	// OPERATOR OVERLOADS
	//-------------------------------------------------------------------------	
		//operator overload for mat3 * mat3
		Mat3  operator * (const Mat3 &rhs) const;
		///operator overlaod for mat3 *= mat3
		Mat3 &operator *=(const Mat3 &rhs); // return *this = *this * rhs;
	
	// TRANSLATION
	//-------------------------------------------------------------------------
		/// only set the translation component of the matrix through float x,y
		void SetTranslation(float x, float y);
		/// only set the translation component of the matrix through Vec2 x,y
		void SetTranslation(Vec2 vec);
		
		/// add x and y onto the translation component of the matrix float x,y
		void Translate(float x, float y);
		/// add x and y onto the translation component of the matrix Vec2
		void Translate(Vec2 vec);  
		
		/// returns the translation component of the matrix
		Vec2 GetTranslation() const;
	
	// ROTATION
	//-------------------------------------------------------------------------
		/// set the rotation of the up and right axis
		void SetRotation(float rad);
		/// set the rotation of the up axis
		void SetUpRotation(float rad);
		/// set the rotation of the right axis
		void SetRightRotation(float rad);
		
		/// multiply the rotation amount to the up and right axis
		void Rotate(float rad);
		/// return the rotation of the up vector away from the worlds up
		float GetUpRot() const;		
		/// return the rotation of the right vector away from the worlds right
		float GetRightRot() const;

	//Get additional info
		///get the current up vector
		Vec2 getUpVec() const;
	
	// SCALE
	//-------------------------------------------------------------------------
		/// set the scale component for the up vec and the right vec
		void SetScale(float x, float y);		
		/// return the scale of the up component of the matrix
		float GetUpScale() const;		
		/// return the scale of the right component of the matrix
		float GetRightScale() const;		
		/// returns the scale as a vector 2
		Vec2 GetScale() const;
	
	// Vector Transform
	//-------------------------------------------------------------------------	
	
	///return the transformed point (x,y) which is the current x,y * the passed vector
	Vec2 TransformPoint(Vec2 const &vec ) const;

	friend class Vec2;

private:
	//Getter Setter member variable functions
		//get mat element - this should be avoided
		float getMatElement(int element) const;
		//set mat element - this should be avoided
		void setMatElement(int element, float value);	
};

#endif MAT3_H