#include "quaternion.h"

//------------------------------------------------------------------------------
/**
*/
Quaternion::Quaternion()
{
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;
	this->data[3] = 1;
}

//------------------------------------------------------------------------------
/**
*/
Quaternion::Quaternion(float angle, const Vector3 &axis)
{
	this->data[0] = axis[0] * sinf((angle) / 2.f);
	this->data[1] = axis[1] * sinf((angle) / 2.f);
	this->data[2] = axis[2] * sinf((angle) / 2.f);
	this->data[3] = cosf((angle) / 2.f);
}

//------------------------------------------------------------------------------
/**
*/
Quaternion::~Quaternion(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Quaternion::Insert( float x, float y, float z, float w )
{
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = w;
}

//------------------------------------------------------------------------------
/**
*/
Quaternion Quaternion::operator*(const Quaternion& v)
{
	Quaternion result;
	
	result[0] = w*v[0] + x*v[3] + y*v[2] - z*v[1]; //x
	result[1] = w*v[1] - x*v[2] + y*v[3] + z*v[0]; //y
	result[2] = w*v[2] + x*v[1] - y*v[0] + z*v[3]; //z
	result[3] = w*v[3] - x*v[0] - y*v[1] - z*v[2]; //w
	
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Quaternion Quaternion::operator*(const float& number)const
{
	Quaternion result;
	for (int i = 0; i < 4; i++){
		result[i] = this->data[i] * number;
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Quaternion::operator*=(const Quaternion& v)
{
	this->data[0] = this->data[3]*v[0] + this->data[0]*v[3] + this->data[1]*v[2] - this->data[2]*v[1]; //x
	this->data[1] = this->data[3]*v[1] - this->data[0]*v[2] + this->data[1]*v[3] + this->data[2]*v[0]; //y
	this->data[2] = this->data[3]*v[2] + this->data[0]*v[1] - this->data[1]*v[0] + this->data[2]*v[3]; //z
	this->data[3] = this->data[3]*v[3] - this->data[0]*v[0] - this->data[1]*v[1] - this->data[2]*v[2]; //w
}

float Quaternion::Magnitude()
{
	return sqrt(x*x + y*y + z*z + w*w);
}

//------------------------------------------------------------------------------
/**
*/
void Quaternion::Normalize()
{	
	float length = Magnitude();
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i]/length;
	}
}

//------------------------------------------------------------------------------
/**
*/
Quaternion Quaternion::Normalized()
{
	Quaternion result;
	
	float length = sqrt(x*x + y*y + z*z + w*w);
	
	for (int i = 0; i < 4; i++)
	{
		result[i] = this->data[i]/length;
	}
	
	return result;
}

Matrix44 Quaternion::ConvertToMatrix()
{
	
	Matrix44 rotation;
	rotation[0][0] = 1- 2* y*y - 2* z*z;
	rotation[0][1] = (2 * x * y) - (2 * w * z);
	rotation[0][2] = (2 * x * z) + (2 * w * y);
	rotation[0][3] = 0;

	rotation[1][0] = (2 * x * y) + (2 * w * z);
	rotation[1][1] = 1- 2* x*x - 2* z*z;
	rotation[1][2] = (2 * y * z) - (2 * w * x);
	rotation[1][3] = 0;

	rotation[2][0] = (2 * x * z) - (2 * w * y);
	rotation[2][1] = (2 * y * z) + (2 * w * x);
	rotation[2][2] = 1- 2* x*x - 2* y*y;
	rotation[2][3] = 0;

	rotation[3][0] = 0;
	rotation[3][1] = 0;
	rotation[3][2] = 0;
	rotation[3][3] = 1;

	return rotation;
}

Matrix33 Quaternion::ConvertToMatrix33()
{
	Matrix33 rotation;
	rotation[0][0] = 1 - 2 * y*y - 2 * z*z;
	rotation[0][1] = (2 * x * y) - (2 * w * z);
	rotation[0][2] = (2 * x * z) + (2 * w * y);

	rotation[1][0] = (2 * x * y) + (2 * w * z);
	rotation[1][1] = 1 - 2 * x*x - 2 * z*z;
	rotation[1][2] = (2 * y * z) - (2 * w * x);

	rotation[2][0] = (2 * x * z) - (2 * w * y);
	rotation[2][1] = (2 * y * z) + (2 * w * x);
	rotation[2][2] = 1 - 2 * x*x - 2 * y*y;

	return rotation;
}

//------------------------------------------------------------------------------
/**
*/
float& Quaternion::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float& Quaternion::operator[](unsigned int index) const
{
	return this->data[index];
}