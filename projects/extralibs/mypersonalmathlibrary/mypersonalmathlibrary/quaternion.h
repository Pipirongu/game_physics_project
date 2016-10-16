#pragma once

#include <cmath>
#include "vector3.h"
#include "matrix44.h"
#include "matrix33.h"

/**
@class Quaternion

Quaternion class
*/
class Quaternion
{
public:
	/// A Vector of size 4 is created with values 0, except w which can be set. Defaults to 1.
	Quaternion();
	/// Constructor to set up a quaternion
	Quaternion(float angle, const Vector3 &axis);
	~Quaternion(void);

	/// Set x, y, z, w values of a Vector. W defaults to 1.
	void Insert(float x, float y, float z, float w=1);

	Quaternion operator*(const Quaternion& v);
	void operator*=(const Quaternion& v);
	Quaternion operator*(const float& number)const;

	float Magnitude();
	/// Normalize this vector
	void Normalize();
	Quaternion Normalized();
	Matrix44 ConvertToMatrix();
	Matrix33 ConvertToMatrix33();
	
	
	/// Overloaded access operator for Vector
	float& operator[](unsigned int index);
	const float& operator[](unsigned int index) const;

private:
	union
	{
		float data[4];
		struct{ float x, y, z, w; };
	};
};
