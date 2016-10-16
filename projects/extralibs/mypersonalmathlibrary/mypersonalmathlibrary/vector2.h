#pragma once

#include <cmath> 

/**
@class Vector2

Vector2 class
*/
class Vector2
{
public:
	Vector2(void);
	/// A Vector of size 2 with x, y values
	Vector2(float x, float y);
	~Vector2(void);

	/// Set x, y values of the Vector
	void Insert(float x, float y);

	/// Vector2+=Vector2. Addition is affecting the left Vector
	void operator+=(const Vector2& rhs);
	/// Vector2+Vector2
	Vector2 operator+(const Vector2& rhs)const;

	/// Vector2-=Vector2. Substraction is affecting the left Vector
	void operator-=(const Vector2& rhs);
	/// Vector2-Vector2
	Vector2 operator-(const Vector2& rhs)const;

	/// Vector2*=number. Multiplication is affecting the left Vector
	void operator*=(const float& number);
	/// Vector2*number
	Vector2 operator*(const float& number)const;

	/// Vector2/=number. Division is affecting the left Vector
	void operator/=(const float& number);
	/// Vector2/number
	Vector2 operator/(const float& number)const;

	/// number*Vector2
	friend Vector2 operator*(const float& number, const Vector2& rhs);

	/// Compares two vectors if they are the same
	bool operator==(const Vector2& rhs)const;

	/// Returns the dot product of this vector
	static float Dot(const Vector2& lhs, const Vector2& rhs);
	/// Returns the length of this vector
	float Magnitude();
	/// Normalize this vector
	void Normalize();
	/// Return the normalized vector
	Vector2 Normalized();

	/// Overloaded access operator for Vector
	float& operator[](unsigned int index);
	const float& operator[](unsigned int index) const;

private:
	union
	{
		float data[2];
		struct{ float x, y; };
	};
};