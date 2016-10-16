#pragma once

#include <cmath>
#include <limits>
class Vector4;

/**
@class Vector3

Vector3 class
*/
class Vector3
{
public:
	union
	{
		struct{ float x, y, z; }; //for easy access
		struct{ signed a, b, c; }; //for hashing a^b^c
		float data[3];
	};
	Vector3(void);
	/// A Vector of size 3 with x, y, z values
	Vector3(float x, float y, float z);
	Vector3(const Vector4 &v);
	~Vector3(void);

	/// Set x, y, z values of the Vector
	void Insert(float x, float y, float z);

	/// Vector3+=Vector3. Addition is affecting the left Vector
	void operator+=(const Vector3& lhs);
	/// Vector3+Vector3
	Vector3 operator+(const Vector3& lhs)const;

	/// Vector3-=Vector3. Substraction is affecting the left Vector
	void operator-=(const Vector3& lhs);
	/// Vector3-Vector3
	Vector3 operator-(const Vector3& lhs)const;

	/// Vector3*=number. Multiplication is affecting the left Vector
	void operator*=(const float& number);
	/// Vector3*number
	Vector3 operator*(const float& number)const;

	/// Vector3/=number. Division is affecting the left Vector
	void operator/=(const float& number);
	/// Vector3/number
	Vector3 operator/(const float& number)const;

	/// number*Vector3
	friend Vector3 operator*(const float& number, const Vector3& lhs);

	/// Compares two vectors if they are the same
	bool operator==(const Vector3& lhs)const;

	/// Compares two vectors if they are different
	bool operator!=(const Vector3& lhs)const;

	/// Returns the dot product of this vector
	static float Dot(const Vector3& lhs, const Vector3& rhs);
	/// Cross Product between two Vectors
	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

	/// Returns the length of this vector
	float Magnitude() const;
	float SquareMagnitude() const;
	/// Normalize this vector
	void Normalize();
	/// Return the normalized vector
	Vector3 Normalized() const;

	/// Overloaded access operator for Vector
	float& operator[](unsigned int index);
	const float& operator[](unsigned int index) const;

private:
	//union
	//{
	//	float data[3];
	//	struct{ float x, y, z; };
	//};
};