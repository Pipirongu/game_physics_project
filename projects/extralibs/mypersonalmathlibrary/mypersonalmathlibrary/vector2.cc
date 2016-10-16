#include "vector2.h"

//------------------------------------------------------------------------------
/**
*/
Vector2::Vector2(void)
{
	this->data[0] = 0;
	this->data[1] = 0;
}

//------------------------------------------------------------------------------
/**
*/
Vector2::Vector2( float x, float y )
{
	this->data[0] = x;
	this->data[1] = y;
}

//------------------------------------------------------------------------------
/**
*/
Vector2::~Vector2(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::Insert( float x, float y )
{
	this->data[0] = x;
	this->data[1] = y;
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::operator+=(const Vector2& rhs)
{
	for (int i = 0; i < 2; i++)
	{
		this->data[i] = this->data[i] + rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector2 Vector2::operator+(const Vector2& rhs)const
{
	Vector2 result;
	for (int i = 0; i < 2; i++){
		result[i] = this->data[i] + rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::operator-=(const Vector2& rhs)
{
	for (int i = 0; i < 2; i++)
	{
		this->data[i] = this->data[i] - rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector2 Vector2::operator-(const Vector2& rhs)const
{
	Vector2 result;
	for (int i = 0; i < 2; i++){
		result[i] = this->data[i] - rhs[i];;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::operator*=(const float& number)
{
	for (int i = 0; i < 2; i++)
	{
		this->data[i] =  this->data[i] * number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector2 Vector2::operator*(const float& number)const
{
	Vector2 result;
	for (int i = 0; i < 2; i++){
		result[i] = this->data[i] * number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::operator/=(const float& number)
{
	for (int i = 0; i < 2; i++)
	{
		this->data[i] = this->data[i] / number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector2 Vector2::operator/(const float& number)const
{
	Vector2 result;
	for (int i = 0; i < 2; i++){
		result[i] = this->data[i] / number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Vector2 operator*(const float& number, const Vector2& rhs)
{
	Vector2 result;
	for (int i = 0; i < 2; i++)
	{
		result[i] = number * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
bool Vector2::operator==(const Vector2& rhs)const
{
	for (int i = 0; i < 2; i++)
	{
		if(this->data[i] != rhs[i]){
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
/**
Multiply respective components of the vectors and add it to result
*/
float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
{
	float result = 0;
	for (int i = 0; i < 2; i++)
	{
		result = result + lhs[i] * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
sqrt(X^2 + Y^2)
*/
float Vector2::Magnitude()
{
	return sqrt(x*x + y*y);
}

//------------------------------------------------------------------------------
/**
*/
void Vector2::Normalize()
{	
	float length = sqrt(x*x + y*y);
	for (int i = 0; i < 2; i++)
	{
		this->data[i] = this->data[i]/length;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector2 Vector2::Normalized()
{
	Vector2 result;
	float length = sqrt(x*x + y*y);
	for (int i = 0; i < 2; i++)
	{
		result[i] = this->data[i]/length;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
float& Vector2::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float& Vector2::operator[](unsigned int index) const
{
	return this->data[index];
}
