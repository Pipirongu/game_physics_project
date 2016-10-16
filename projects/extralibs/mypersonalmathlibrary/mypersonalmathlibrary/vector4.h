#pragma once

#include <cmath> 
class Vector3;

/**
@class Vector4

Vector4 class
*/
class Vector4
{
public:
	/// A Vector of size 4 is created with values 0, except w which can be set. Defaults to 1.
	Vector4(float w=1);
	/// A Vector of size 4 with x, y, z, w values. W defaults to 1.
	Vector4(float x, float y, float z, float w=1);
	Vector4(const Vector3 &v, float w = 1);
	~Vector4(void);

	/// Set x, y, z, w values of a Vector. W defaults to 1.
	void Insert(float x, float y, float z, float w=1);

	/// Vector4+=Vector4. Addition is affecting the left Vector
	void operator+=(const Vector4& rhs);
	/// Vector4+Vector4
	Vector4 operator+(const Vector4& rhs)const;

	/// Vector4-=Vector4. Substraction is affecting the left Vector
	void operator-=(const Vector4& rhs);
	/// Vector4-Vector4
	Vector4 operator-(const Vector4& rhs)const;

	/// Vector4*=number. Multiplication is affecting the left Vector
	void operator*=(const float& number);
	/// Vector4*number
	Vector4 operator*(const float& number)const;

	/// Vector4/=number. Division is affecting the left Vector
	void operator/=(const float& number);
	/// Vector4/number
	Vector4 operator/(const float& number)const;

	/// number*Vector4
	friend Vector4 operator*(const float& number, const Vector4& rhs);

	/// Compares two vectors if they are the same
	bool operator==(const Vector4& rhs)const;

	/// Returns the dot product of this vector
	static float Dot(const Vector4& lhs, const Vector4& rhs);
	/// Returns the length of this vector
	float Magnitude();
	/// Normalize this vector
	void Normalize();
	/// Return the normalized vector
	Vector4 Normalized();

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