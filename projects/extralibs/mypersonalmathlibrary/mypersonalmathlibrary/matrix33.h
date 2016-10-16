#pragma once

#include "vector3.h"
#include "mypersonalmathlibconstants.h"

#include <cmath>
#include <algorithm>

/**
@class Matrix33

A Matrix33 class
*/
class Matrix33
{
public:
	Matrix33(void);
	~Matrix33(void);

	/// Sets a Row of elements in the Matrix
	void SetRow(unsigned int row, const Vector3 &v);
	/// Sets a Column of elements in the Matrix
	void SetColumn(unsigned int column, const Vector3 &v);
	/// Sets a Row of elements in the Matrix
	void SetRow(unsigned int row, float x, float y, float z);
	/// Sets a Column of elements in the Matrix
	void SetColumn(unsigned int column, float x, float y, float z);
	
	/// Sets this matrix to an Identity Matrix
	void SetToIdentity();

	/// Matrix33*=Matrix33. Multiplication is affecting the left Matrix
	void operator*=(const Matrix33& rhs);
	/// Matrix33*Matrix33
	Matrix33 operator*(const Matrix33& rhs)const;

	/// Matrix33*Vector3
	Vector3 operator*(const Vector3& rhs)const;

	/// Matrix33*=number. Multiplication is affecting the Matrix
	void operator*=(const float& number);
	/// Matrix33*number
	Matrix33 operator*(const float& number)const;

	/// number*Matrix
	friend Matrix33 operator*(const float& number, const Matrix33& rhs);

	/// Compares two Matrices if they are the same
	bool operator==(const Matrix33& rhs)const;
	
	/// Transpose this Matrix, switching rows to columns
	void Transpose();
	Matrix33 Transposed();
	/// Returns the Determinant of a matrix
	float Determinant();
	/// Returns the Inverse of a Matrix. Only possible if (Determinant != 0). Returns the Identity Matrix if Determinant = 0
	Matrix33 Inverse() const;
	static Matrix33 InertiaTensor(float mass, const Vector3 &dimensions);
	static Matrix33 ZeroMatrix();

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

	/// Overloaded access operator for a Matrix
	float* operator[](unsigned int index);
	const float* operator[](unsigned int index) const;
	
private:
	//holds matrix33
	float data[3][3];
};