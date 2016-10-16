#pragma once

#include "matrix33.h"
#include "vector4.h"
#include "vector3.h"
#include "mypersonalmathlibconstants.h"

#include <cmath> 

/**
@class Matrix44

A Matrix44 class
*/
class Matrix44
{
public:
	Matrix44(void);
	~Matrix44(void);

	/// Sets a Row of elements in the Matrix
	void SetRow(unsigned int row, const Vector4 &v);
	/// Sets a Column of elements in the Matrix
	void SetColumn(unsigned int column, const Vector4 &v);
	/// Sets a Row of elements in the Matrix
	void SetRow(unsigned int row, float x, float y, float z, float w);
	/// Sets a Column of elements in the Matrix
	void SetColumn(unsigned int column, float x, float y, float z, float w);

	/// Sets this matrix to an Identity Matrix
	void SetToIdentity();
	static Matrix44 ZeroMatrix();

	/// Matrix44*=Matrix44. Multiplication is affecting the left Matrix
	void operator*=(const Matrix44& rhs);
	/// Matrix44*Matrix44
	Matrix44 operator*(const Matrix44& rhs)const;

	/// Matrix44*Vector4
	Vector4 operator*(const Vector4& rhs)const;

	/// Matrix44*=number. Multiplication is affecting the Matrix
	void operator*=(const float& number);
	/// Matrix44*number
	Matrix44 operator*(const float& number)const;

	/// number*Matrix44
	friend Matrix44 operator*(const float& number, const Matrix44& rhs);

	/// Compares two Matrices if they are the same
	bool operator==(const Matrix44& rhs)const;

	/// Compares two Matrices if they are different
	bool operator!=(const Matrix44& rhs)const;

	/// Transpose the Matrix, switching rows to columns
	void Transpose();
	Matrix44 Transposed();

	/// Returns the Determinant of a matrix
	float Determinant();
	/// Returns the Inverse of a Matrix. Only possible if (Determinant != 0). Returns the Identity Matrix if Determinant = 0
	Matrix44 Inverse();

	/// Multiply the matrix with a Rotation matrix around X axis
	void RotateAroundX(float angle);
	/// Multiply the matrix with a Rotation matrix around Y axis
	void RotateAroundY(float angle);
	/// Multiply the matrix with a Rotation matrix around Z axis
	void RotateAroundZ(float angle);

	/// Multiply the matrix with a Rotation matrix. Around an arbitary axis
	void Rotate(float angle, int x, int y, int z);
	/// Multiply the matrix with a Rotation matrix. Around an arbitary axis
	void Rotate(float angle, const Vector3 &v);

	/// Multiply the matrix with a Translation matrix.
	void Translate(float x, float y, float z);
	/// Multiply the matrix with a Translation matrix.
	void Translate(const Vector3 &v);
	void Translate(const Vector4 &v);

	static Matrix44 Translation(const Vector4 &v);
	static Matrix44 Translation(const Vector3 &v);

	/// Multiply the matrix with a Scaling matrix. Scales equally for xyz axis
	void Scale(float factor);
	/// Multiply the matrix with a Scaling matrix.
	void Scale(float x, float y, float z);
	/// Multiply the matrix with a Scaling matrix.
	void Scale(const Vector3 &v);

	/// lookAt function meant for camera
	void LookAt(const Vector3 &eyePosition, const Vector3 &eyeTarget, const Vector3 &eyeUp);

	/// Sets the matrix to a Perspective projection matrix
	static Matrix44 Perspective(float angle, float aspect, float near, float far);
	/// Sets the matrix to an Orthographic projection matrix
	static Matrix44 Ortho();
	static Matrix44 Ortho(float left, float right, float bottom, float top, float near, float far);

	/// Overloaded access operator for a Matrix
	float* operator[](unsigned int index);
	const float* operator[](unsigned int index) const;

	Matrix33 ConvertToMatrix33();

private:
	//Matrix 4x4
	float data[4][4];
};