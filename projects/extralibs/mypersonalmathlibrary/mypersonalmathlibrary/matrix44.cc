#include "matrix44.h"

//------------------------------------------------------------------------------
/**
*/
Matrix44::Matrix44(void)
{
	memset(this->data, 0, sizeof(float) * 16);

	//Set matrix values to identity
	this->data[0][0] = 1;
	this->data[1][1] = 1;
	this->data[2][2] = 1;
	this->data[3][3] = 1;
}

//------------------------------------------------------------------------------
/**
*/
Matrix44::~Matrix44(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::SetRow(unsigned int row, const Vector4 &v)
{
	this->data[row][0] = v[0];
	this->data[row][1] = v[1];
	this->data[row][2] = v[2];
	this->data[row][3] = v[3];
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::SetColumn(unsigned int column, const Vector4 &v)
{
	this->data[0][column] = v[0];
	this->data[1][column] = v[1];
	this->data[2][column] = v[2];
	this->data[3][column] = v[3];
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::SetRow( unsigned int row, float x, float y, float z, float w )
{
	this->data[row][0] = x;
	this->data[row][1] = y;
	this->data[row][2] = z;
	this->data[row][3] = w;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::SetColumn( unsigned int column, float x, float y, float z, float w )
{
	this->data[0][column] = x;
	this->data[1][column] = y;
	this->data[2][column] = z;
	this->data[3][column] = w;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::SetToIdentity()
{
	//Reset the values to 0
	memset(this->data, 0, sizeof(float) * 16);

	//Set the matrix to an identity matrix
	this->data[0][0] = 1;
	this->data[1][1] = 1;
	this->data[2][2] = 1;
	this->data[3][3] = 1;
}

Matrix44 Matrix44::ZeroMatrix()
{
	Matrix44 zero_matrix;
	zero_matrix[0][0] = 0;
	zero_matrix[1][1] = 0;
	zero_matrix[2][2] = 0;
	zero_matrix[3][3] = 0;

	return zero_matrix;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::operator*=(const Matrix44& rhs)
{
	//Temporary save the left matrix
	Matrix44 temp = *this;

	//Reset the values of the left matrix to 0
	memset(this->data, 0, sizeof(float) * 16);

	//Multiplies each component of the left matrix row with each component of the right matrix column
	//Each multiplication is added to the index of the result matrix.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				this->data[i][j] = this->data[i][j] + temp[i][k] * rhs[k][j];
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::operator*(const Matrix44& rhs)const
{
	Matrix44 result;

	//Fill result matrix with 0
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = 0;	
		}
	}

	//Multiplies each component of the left matrix row with each component of the right matrix column
	//Each multiplication is added to the index of the result matrix.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] = result[i][j] + this->data[i][k] * rhs[k][j];
			}
		}
	}	
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Vector4 Matrix44::operator*(const Vector4& rhs)const
{
 	//Initialized with x,y,z,w = 0
 	Vector4 result;
 
 	//Multiplies each component of the left matrix row with each component of the right vector column
 	//Each multiplication is added to the index of the result vector.
 	for (int i = 0; i < 4; i++)
 	{
 		for (int j = 0; j < 4; j++)
 		{
 			result[i] = result[i] + this->data[i][j] * rhs[j];
 		}
 	}	
 	return result;
	
	//float vx = rhs[0];
	//float vy = rhs[1];
	//float vz = rhs[2];
	//float vw = rhs[3];
	//
	//float _x = this->data[0][0] * vx + this->data[1][0] * vy + this->data[2][0] * vz + this->data[3][0] * vw;
	//float _y = this->data[0][1] * vx + this->data[1][1] * vy + this->data[2][1] * vz + this->data[3][1] * vw;
	//float _z = this->data[0][2] * vx + this->data[1][2] * vy + this->data[2][2] * vz + this->data[3][2] * vw;
	//float _w = this->data[0][3] * vx + this->data[1][3] * vy + this->data[2][3] * vz + this->data[3][3] * vw;
	//Vector4 newVector(_x, _y, _z, _w);
	//return newVector;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::operator*=(const float& number)
{
	Matrix44 temp = *this;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->data[i][j] = temp[i][j] * number;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::operator*(const float& number)const
{
	Matrix44 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = this->data[i][j] * number;
		}
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 operator*(const float& number, const Matrix44& rhs)
{
	Matrix44 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = number * rhs[i][j];
		}
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
bool Matrix44::operator==(const Matrix44& rhs)const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (this->data[i][j] != rhs[i][j]){
				return false;
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool Matrix44::operator!=(const Matrix44& rhs)const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (this->data[i][j] != rhs[i][j]){
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Transpose()
{
	Matrix44 temp = *this;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->data[i][j] = temp[j][i];
		}
	}
}

Matrix44 Matrix44::Transposed()
{
	Matrix44 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = this->data[j][i];
		}
	}

	return result;
}

//------------------------------------------------------------------------------
/**
*/
float Matrix44::Determinant()
{
	return +this->data[0][0]*this->data[1][1]*this->data[2][2]*this->data[3][3]
			+this->data[0][0]*this->data[1][2]*this->data[2][3]*this->data[3][1]
			+this->data[0][0]*this->data[1][3]*this->data[2][1]*this->data[3][2]

			+this->data[0][1]*this->data[1][0]*this->data[2][3]*this->data[3][2]
			+this->data[0][1]*this->data[1][2]*this->data[2][0]*this->data[3][3]
			+this->data[0][1]*this->data[1][3]*this->data[2][2]*this->data[3][0]

			+this->data[0][2]*this->data[1][0]*this->data[2][1]*this->data[3][3]
			+this->data[0][2]*this->data[1][1]*this->data[2][3]*this->data[3][0]
			+this->data[0][2]*this->data[1][3]*this->data[2][0]*this->data[3][1]

			
			+this->data[0][3]*this->data[1][0]*this->data[2][2]*this->data[3][1]
			+this->data[0][3]*this->data[1][1]*this->data[2][0]*this->data[3][2]
			+this->data[0][3]*this->data[1][2]*this->data[2][1]*this->data[3][0]

			
			-this->data[0][0]*this->data[1][1]*this->data[2][3]*this->data[3][2]
			-this->data[0][0]*this->data[1][2]*this->data[2][1]*this->data[3][3]
			-this->data[0][0]*this->data[1][3]*this->data[2][2]*this->data[3][1]

			-this->data[0][1]*this->data[1][0]*this->data[2][2]*this->data[3][3]
			-this->data[0][1]*this->data[1][2]*this->data[2][3]*this->data[3][0]
			-this->data[0][1]*this->data[1][3]*this->data[2][0]*this->data[3][2]

			-this->data[0][2]*this->data[1][0]*this->data[2][3]*this->data[3][1]
			-this->data[0][2]*this->data[1][1]*this->data[2][0]*this->data[3][3]
			-this->data[0][2]*this->data[1][3]*this->data[2][1]*this->data[3][0]

			-this->data[0][3]*this->data[1][0]*this->data[2][1]*this->data[3][2]
			-this->data[0][3]*this->data[1][1]*this->data[2][2]*this->data[3][0]
			-this->data[0][3]*this->data[1][2]*this->data[2][0]*this->data[3][1];

}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::Inverse()
{
	Matrix44 result = *this;
	float det = result.Determinant();
	if(det == 0)
	{	
		printf("Determinant = 0, no Inverse for this matrix. Matrix Set to Zero\n");
		result.ZeroMatrix();
		return result;
	}
	result[0][0] =  (this->data[1][1]*this->data[2][2]*this->data[3][3]
					+this->data[1][2]*this->data[2][3]*this->data[3][1]
					+this->data[1][3]*this->data[2][1]*this->data[3][2]
					-this->data[1][1]*this->data[2][3]*this->data[3][2]
					-this->data[1][2]*this->data[2][1]*this->data[3][3]
					-this->data[1][3]*this->data[2][2]*this->data[3][1]);

	result[0][1] =  (this->data[0][1]*this->data[2][3]*this->data[3][2]
					+this->data[0][2]*this->data[2][1]*this->data[3][3]
					+this->data[0][3]*this->data[2][2]*this->data[3][1]
					-this->data[0][1]*this->data[2][2]*this->data[3][3]
					-this->data[0][2]*this->data[2][3]*this->data[3][1]
					-this->data[0][3]*this->data[2][1]*this->data[3][2]);

	result[0][2] =  (this->data[0][1]*this->data[1][2]*this->data[3][3]
					+this->data[0][2]*this->data[1][3]*this->data[3][1]
					+this->data[0][3]*this->data[1][1]*this->data[3][2]
					-this->data[0][1]*this->data[1][3]*this->data[3][2]
					-this->data[0][2]*this->data[1][1]*this->data[3][3]
					-this->data[0][3]*this->data[1][2]*this->data[3][1]);

	result[0][3] =  (this->data[0][1]*this->data[1][3]*this->data[2][2]
					+this->data[0][2]*this->data[1][1]*this->data[2][3]
					+this->data[0][3]*this->data[1][2]*this->data[2][1]
					-this->data[0][1]*this->data[1][2]*this->data[2][3]
					-this->data[0][2]*this->data[1][3]*this->data[2][1]
					-this->data[0][3]*this->data[1][1]*this->data[2][2]);

	result[1][0] =  (this->data[1][0]*this->data[2][3]*this->data[3][2]
					+this->data[1][2]*this->data[2][0]*this->data[3][3]
					+this->data[1][3]*this->data[2][2]*this->data[3][0]
					-this->data[1][0]*this->data[2][2]*this->data[3][3]
					-this->data[1][2]*this->data[2][3]*this->data[3][0]
					-this->data[1][3]*this->data[2][0]*this->data[3][2]);

	result[1][1] =  (this->data[0][0]*this->data[2][2]*this->data[3][3]
					+this->data[0][2]*this->data[2][3]*this->data[3][0]
					+this->data[0][3]*this->data[2][0]*this->data[3][2]
					-this->data[0][0]*this->data[2][3]*this->data[3][2]
					-this->data[0][2]*this->data[2][0]*this->data[3][3]
					-this->data[0][3]*this->data[2][2]*this->data[3][0]);

	result[1][2] =  (this->data[0][0]*this->data[1][3]*this->data[3][2]
					+this->data[0][2]*this->data[1][0]*this->data[3][3]
					+this->data[0][3]*this->data[1][2]*this->data[3][0]
					-this->data[0][0]*this->data[1][2]*this->data[3][3]
					-this->data[0][2]*this->data[1][3]*this->data[3][0]
					-this->data[0][3]*this->data[1][0]*this->data[3][2]);

	result[1][3] =  (this->data[0][0]*this->data[1][2]*this->data[2][3]
					+this->data[0][2]*this->data[1][3]*this->data[2][0]
					+this->data[0][3]*this->data[1][0]*this->data[2][2]
					-this->data[0][0]*this->data[1][3]*this->data[2][2]
					-this->data[0][2]*this->data[1][0]*this->data[2][3]
					-this->data[0][3]*this->data[1][2]*this->data[2][0]);

	result[2][0] =  (this->data[1][0]*this->data[2][1]*this->data[3][3]
					+this->data[1][1]*this->data[2][3]*this->data[3][0]
					+this->data[1][3]*this->data[2][0]*this->data[3][1]
					-this->data[1][0]*this->data[2][3]*this->data[3][1]
					-this->data[1][1]*this->data[2][0]*this->data[3][3]
					-this->data[1][3]*this->data[2][1]*this->data[3][0]);

	result[2][1] =  (this->data[0][0]*this->data[2][3]*this->data[3][1]
					+this->data[0][1]*this->data[2][0]*this->data[3][3]
					+this->data[0][3]*this->data[2][1]*this->data[3][0]
					-this->data[0][0]*this->data[2][1]*this->data[3][3]
					-this->data[0][1]*this->data[2][3]*this->data[3][0]
					-this->data[0][3]*this->data[2][0]*this->data[3][1]);

	result[2][2] =  (this->data[0][0]*this->data[1][1]*this->data[3][3]
					+this->data[0][1]*this->data[1][3]*this->data[3][0]
					+this->data[0][3]*this->data[1][0]*this->data[3][1]
					-this->data[0][0]*this->data[1][3]*this->data[3][1]
					-this->data[0][1]*this->data[1][0]*this->data[3][3]
					-this->data[0][3]*this->data[1][1]*this->data[3][0]);

	result[2][3] =  (this->data[0][0]*this->data[1][3]*this->data[2][1]
					+this->data[0][1]*this->data[1][0]*this->data[2][3]
					+this->data[0][3]*this->data[1][1]*this->data[2][0]
					-this->data[0][0]*this->data[1][1]*this->data[2][3]
					-this->data[0][1]*this->data[1][3]*this->data[2][0]
					-this->data[0][3]*this->data[1][0]*this->data[2][1]);

	result[3][0] =  (this->data[1][0]*this->data[2][2]*this->data[3][1]
					+this->data[1][1]*this->data[2][0]*this->data[3][2]
					+this->data[1][2]*this->data[2][1]*this->data[3][0]
					-this->data[1][0]*this->data[2][1]*this->data[3][2]
					-this->data[1][1]*this->data[2][2]*this->data[3][0]
					-this->data[1][2]*this->data[2][0]*this->data[3][1]);

	result[3][1] =  (this->data[0][0]*this->data[2][1]*this->data[3][2]
					+this->data[0][1]*this->data[2][2]*this->data[3][0]
					+this->data[0][2]*this->data[2][0]*this->data[3][1]
					-this->data[0][0]*this->data[2][2]*this->data[3][1]
					-this->data[0][1]*this->data[2][0]*this->data[3][2]
					-this->data[0][2]*this->data[2][1]*this->data[3][0]);

	result[3][2] =  (this->data[0][0]*this->data[1][2]*this->data[3][1]
					+this->data[0][1]*this->data[1][0]*this->data[3][2]
					+this->data[0][2]*this->data[1][1]*this->data[3][0]
					-this->data[0][0]*this->data[1][1]*this->data[3][2]
					-this->data[0][1]*this->data[1][2]*this->data[3][0]
					-this->data[0][2]*this->data[1][0]*this->data[3][1]);

	result[3][3] =	(this->data[0][0]*this->data[1][1]*this->data[2][2]
					+this->data[0][1]*this->data[1][2]*this->data[2][0]
					+this->data[0][2]*this->data[1][0]*this->data[2][1]
					-this->data[0][0]*this->data[1][2]*this->data[2][1]
					-this->data[0][1]*this->data[1][0]*this->data[2][2]
					-this->data[0][2]*this->data[1][1]*this->data[2][0]);
	result = (1/det)*result;

	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::RotateAroundX(float angle)
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][2] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::RotateAroundY(float angle)
{
	Matrix44 result;
	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][1] = 0;
	result[0][2] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = 0;
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::RotateAroundZ(float angle)
{
	Matrix44 result;
	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][1] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *=result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Rotate(float angle, int x, int y, int z)
{
	Matrix44 result;

	Vector3 inputVector(x, y, z);
	inputVector.Normalize();

	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[0], 2);
	result[0][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] - inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] + inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][3] = 0;

	result[1][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] + inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[1], 2);
	result[1][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] - inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][3] = 0;

	result[2][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] - inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] + inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[2], 2);
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Rotate(float angle, const Vector3 &v)
{
	Matrix44 result;

	Vector3 inputVector = v;
	inputVector.Normalize();

	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[0], 2);
	result[0][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] - inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] + inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][3] = 0;

	result[1][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] + inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[1], 2);
	result[1][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] - inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][3] = 0;

	result[2][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] - inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] + inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[2], 2);
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Translate( float x, float y, float z )
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = x;

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = y;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = z;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Translate(const Vector3 &v)
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = v[0];

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = v[1];

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = v[2];

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

void Matrix44::Translate(const Vector4 &v)
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = v[0];

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = v[1];

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = v[2];

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

Matrix44 Matrix44::Translation(const Vector4 &v)
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = v[0];

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = v[1];

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = v[2];

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}

Matrix44 Matrix44::Translation(const Vector3 &v)
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = v[0];

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = v[1];

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;
	result[2][3] = v[2];

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Scale( float factor )
{
	Matrix44 result;
	result[0][0] = factor;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = factor;
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = factor;
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Scale( float x, float y, float z )
{
	Matrix44 result;
	result[0][0] = x;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = y;
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = z;
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::Scale(const Vector3 &v)
{
	Matrix44 result;
	result[0][0] = v[0];
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = v[1];
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = v[2];
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix44::LookAt(const Vector3 &eyePosition, const Vector3 &eyeTarget, const Vector3 &eyeUp)
{
	Vector3 forward = (eyeTarget - eyePosition).Normalized();

	Vector3 right = (Vector3::Cross(forward, eyeUp)).Normalized();

	Vector3 up = (Vector3::Cross(right, forward)).Normalized();

	Matrix44 rotation;

	rotation[0][0] = right[0];
	rotation[0][1] = right[1];
	rotation[0][2] = right[2];
	rotation[0][3] = 0;

	rotation[1][0] = up[0];
	rotation[1][1] = up[1];
	rotation[1][2] = up[2];
	rotation[1][3] = 0;

	rotation[2][0] = -forward[0];
	rotation[2][1] = -forward[1];
	rotation[2][2] = -forward[2];
	rotation[2][3] = 0;

	rotation[3][0] = 0;
	rotation[3][1] = 0;
	rotation[3][2] = 0;
	rotation[3][3] = 1;

	Matrix44 translation;
	translation.Translate(-eyePosition[0], -eyePosition[1], -eyePosition[2]);

	*this = rotation*translation;
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::Perspective(float angle, float aspect, float near, float far)
{
	Matrix44 result;

	float f;
	//cot = cosf(x)/sinf(x) or 1/tan(x)
	f = 1/tan((angle/2)*MyPersonalMathLibraryConstants::PI/180.0f); //cosf(angle/2)/sinf(angle/2); 
	result[0][0] = f/aspect;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = f;
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = (far+near)/(near-far);
	result[2][3] = (2*far*near)/(near-far);

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = -1;
	result[3][3] = 0;

	return result;
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::Ortho()
{
	Matrix44 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = 0;

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;
	result[1][3] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 0;
	result[2][3] = 0;

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Matrix44::Ortho( float left, float right, float bottom, float top, float near, float far ) //-aspect, aspect, -1, 1, 0, 100
{
	Matrix44 result;
	result[0][0] = 2 / (right - left);
	result[0][1] = 0;
	result[0][2] = 0;
	result[0][3] = -((right + left) / (right - left));

	result[1][0] = 0;
	result[1][1] = 2 / (top - bottom);
	result[1][2] = 0;
	result[1][3] = -((top + bottom) / (top - bottom));

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = -2 / (far - near);
	result[2][3] = -((far + near) / (far - near));

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}

//------------------------------------------------------------------------------
/**
*/
float* Matrix44::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float* Matrix44::operator[](unsigned int index) const
{
	return this->data[index];
}

Matrix33 Matrix44::ConvertToMatrix33()
{
	Matrix33 result;
	result[0][0] = this->data[0][0];
	result[0][1] = this->data[0][1];
	result[0][2] = this->data[0][2];

	result[1][0] = this->data[1][0];
	result[1][1] = this->data[1][1];
	result[1][2] = this->data[1][2];

	result[2][0] = this->data[2][0];
	result[2][1] = this->data[2][1];
	result[2][2] = this->data[2][2];

	return result;
}

