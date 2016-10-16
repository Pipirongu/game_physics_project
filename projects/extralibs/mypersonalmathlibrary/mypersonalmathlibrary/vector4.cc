#include "vector4.h"
#include "vector3.h"

//------------------------------------------------------------------------------
/**
*/
Vector4::Vector4(float w)
{
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;
	this->data[3] = w;
}

//------------------------------------------------------------------------------
/**
*/
Vector4::Vector4( float x, float y, float z, float w )
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = w;
}

Vector4::Vector4(const Vector3 &v, float w)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
	this->data[3] = w;
}

//------------------------------------------------------------------------------
/**
*/
Vector4::~Vector4(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Vector4::Insert( float x, float y, float z, float w )
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = w;
}
//------------------------------------------------------------------------------
/**
*/
void Vector4::operator+=(const Vector4& rhs)
{
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i] + rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Vector4::operator+(const Vector4& rhs)const
{
	Vector4 result;
	for (int i = 0; i < 4; i++){
		result[i] = this->data[i] + rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector4::operator-=(const Vector4& rhs)
{
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i] - rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Vector4::operator-(const Vector4& rhs)const
{
	Vector4 result;
	for (int i = 0; i < 4; i++){
		result[i] = this->data[i] - rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector4::operator*=(const float& number)
{
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i] * number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Vector4::operator*(const float& number)const
{
	Vector4 result;
	for (int i = 0; i < 4; i++){
		result[i] = this->data[i] * number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector4::operator/=(const float& number)
{
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i] / number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Vector4::operator/(const float& number)const
{
	Vector4 result;
	for (int i = 0; i < 4; i++){
		result[i] = this->data[i] / number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Vector4 operator*(const float& number, const Vector4& rhs)
{
	Vector4 result;
	for (int i = 0; i < 4; i++)
	{
		result[i] = number * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
bool Vector4::operator==(const Vector4& rhs)const
{
	for (int i = 0; i < 4; i++)
	{
		if (this->data[i] != rhs[i]){
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
/**
Multiply respective components of the vectors and add it to result
*/
float Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
{
	float result = 0;
	for (int i = 0; i < 4; i++)
	{
		result = result + lhs[i] * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
sqrt(X^2 + Y^2 + Z^2 + W^2)
*/
float Vector4::Magnitude()
{
	return sqrt(x*x + y*y + z*z + w*w);
}

//------------------------------------------------------------------------------
/**
*/
void Vector4::Normalize()
{	
	float length = sqrt(x*x + y*y + z*z + w*w);
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i] / length;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Vector4::Normalized()
{
	Vector4 result;
	float length = sqrt(x*x + y*y + z*z + w*w);
	for (int i = 0; i < 4; i++)
	{
		result[i] = this->data[i] / length;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
float& Vector4::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float& Vector4::operator[](unsigned int index) const
{
	return this->data[index];
}
