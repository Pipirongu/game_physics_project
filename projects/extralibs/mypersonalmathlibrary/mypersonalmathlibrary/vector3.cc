#include "vector3.h"
#include "vector4.h"

//------------------------------------------------------------------------------
/**
*/
Vector3::Vector3(void)
{
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;
}

//------------------------------------------------------------------------------
/**
*/
Vector3::Vector3( float x, float y, float z )
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
}

Vector3::Vector3(const Vector4& v)
{
	this->data[0] = v[0];
	this->data[1] = v[1];
	this->data[2] = v[2];
}

//------------------------------------------------------------------------------
/**
*/
Vector3::~Vector3(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::Insert( float x, float y, float z )
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::operator+=(const Vector3& rhs)
{
	for (int i = 0; i < 3; i++)
	{
		this->data[i] = this->data[i] + rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector3 Vector3::operator+(const Vector3& rhs)const
{
	Vector3 result;
	for (int i = 0; i < 3; i++){
		result[i] = this->data[i] + rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::operator-=(const Vector3& rhs)
{
	for (int i = 0; i < 3; i++)
	{
		this->data[i] = this->data[i] - rhs[i];
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector3 Vector3::operator-(const Vector3& rhs)const
{
	Vector3 result;
	for (int i = 0; i < 3; i++){
		result[i] = this->data[i] - rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::operator*=(const float& number)
{
	for (int i = 0; i < 3; i++)
	{
		this->data[i] = this->data[i] * number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector3 Vector3::operator*(const float& number)const
{
	Vector3 result;
	for (int i = 0; i < 3; i++){
		result[i] = this->data[i] * number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::operator/=(const float& number)
{
	for (int i = 0; i < 3; i++)
	{
		this->data[i] = this->data[i] / number;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector3 Vector3::operator/(const float& number)const
{
	Vector3 result;
	for (int i = 0; i < 3; i++){
		result[i] = this->data[i] / number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Vector3 operator*(const float& number, const Vector3& rhs)
{
	Vector3 result;
	for (int i = 0; i < 3; i++)
	{
		result[i] = number * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
bool Vector3::operator==(const Vector3& rhs)const
{
	for (int i = 0; i < 3; i++)
	{
		if (this->data[i] != rhs[i]){
			return false;
		}
	}
	return true;

	//float EPSILON = 0.001;// std::numeric_limits<float>::epsilon();
	//if (abs(this->data[0] - rhs[0]) < EPSILON &&
	//	abs(this->data[1] - rhs[1]) < EPSILON &&
	//	abs(this->data[2] - rhs[2]) < EPSILON)
	//{
	//	return true; //Almost equal
	//}

	//return false; //The points were not equal enough
}

//------------------------------------------------------------------------------
/**
*/
bool Vector3::operator!=(const Vector3& rhs)const
{
	for (int i = 0; i < 3; i++)
	{
		if (this->data[i] != rhs[i]){
			return true;
		}
	}
	return false;
}


//------------------------------------------------------------------------------
/**
Multiply respective components of the vectors and add it to result
*/
float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	float result = 0;
	for (int i = 0; i < 3; i++)
	{
		result = result + lhs[i] * rhs[i];
	}
	return result;
}

//------------------------------------------------------------------------------
/**
sqrt(X^2 + Y^2 + Z^2)
*/
float Vector3::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}

float Vector3::SquareMagnitude() const
{
	return (x*x + y*y + z*z);
}

//------------------------------------------------------------------------------
/**
*/
void Vector3::Normalize()
{	
	float length = sqrt(x*x + y*y + z*z);
	for (int i = 0; i < 3; i++)
	{
		this->data[i] = this->data[i]/length;
	}
}

//------------------------------------------------------------------------------
/**
*/
Vector3 Vector3::Normalized() const
{
	Vector3 result;
	float length = sqrt(x*x + y*y + z*z);
	for (int i = 0; i < 3; i++)
	{
		result[i] = this->data[i]/length;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
///Cross Product between two Vectors, using the "pencil" rule
*/
Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	Vector3 result;
	result[0] = (lhs[1]*rhs[2]) - (lhs[2]*rhs[1]);
	result[1] = -((lhs[0]*rhs[2]) - (lhs[2]*rhs[0]));
	result[2] = (lhs[0]*rhs[1]) - (lhs[1]*rhs[0]);
	
	return result;
}

//------------------------------------------------------------------------------
/**
*/
float& Vector3::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float& Vector3::operator[](unsigned int index) const
{
	return this->data[index];
}