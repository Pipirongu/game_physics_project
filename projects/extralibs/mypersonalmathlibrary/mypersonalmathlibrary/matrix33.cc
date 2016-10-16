#include "matrix33.h"

//------------------------------------------------------------------------------
/**
*/
Matrix33::Matrix33(void)
{
	memset(this->data, 0, sizeof(float) * 9);

	//Set this->data values to identity
	this->data[0][0] = 1;
	this->data[1][1] = 1;
	this->data[2][2] = 1;
}

//------------------------------------------------------------------------------
/**
*/
Matrix33::~Matrix33(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::SetRow(unsigned int row, const Vector3 &v)
{
	this->data[row][0] = v[0];
	this->data[row][1] = v[1];
	this->data[row][2] = v[2];
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::SetColumn(unsigned int column, const Vector3 &v)
{
	this->data[0][column] = v[0];
	this->data[1][column] = v[1];
	this->data[2][column] = v[2];
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::SetRow( unsigned int row, float x, float y, float z )
{
	this->data[row][0] = x;
	this->data[row][1] = y;
	this->data[row][2] = z;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::SetColumn( unsigned int column, float x, float y, float z )
{
	this->data[0][column] = x;
	this->data[1][column] = y;
	this->data[2][column] = z;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::SetToIdentity()
{
	//Reset the values to 0
	memset(this->data, 0, sizeof(float) * 9);

	//Set the this->data to an identity this->data
	this->data[0][0] = 1;
	this->data[1][1] = 1;
	this->data[2][2] = 1;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::operator*=(const Matrix33& rhs)
{
	//Temporary save the left matrix
	Matrix33 temp = *this;

	//Reset the left matrix with 0
	memset(this->data, 0, sizeof(float) * 9);

	//Multiplies each component of the left matrix row with each component of the right matrix column
	//Each multiplication is added to the index of the result matrix.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				this->data[i][j] = this->data[i][j] + temp[i][k] * rhs[k][j];
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
Matrix33 Matrix33::operator*(const Matrix33& rhs)const
{
	Matrix33 result;

	//Fill the result matrix with 0
	result[0][0] = 0;
	result[1][1] = 0;
	result[2][2] = 0;

	//Multiplies each component of the left matrix row with each component of the right matrix column
	//Each multiplication is added to the index of the result matrix.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
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
Vector3 Matrix33::operator*(const Vector3& rhs)const
{
 	//Initialized with x,y,z = 0
 	Vector3 result;
 
 	//Multiplies each component of the left matrix row with each component of the right vector column
 	//Each multiplication is added to the index of the result vector.
 	for (int i = 0; i < 3; i++)
 	{
 		for (int j = 0; j < 3; j++)
 		{
 			result[i] = result[i] + this->data[i][j] * rhs[j];
 		}
 	}
 	return result;
	
	//float vx = rhs[0];
	//float vy = rhs[1];
	//float vz = rhs[2];
	//
	//float _x = this->data[0][0] * vx + this->data[1][0] * vy + this->data[2][0] * vz;
	//float _y = this->data[0][1] * vx + this->data[1][1] * vy + this->data[2][1] * vz;
	//float _z = this->data[0][2] * vx + this->data[1][2] * vy + this->data[2][2] * vz;
	//Vector3 newVector(_x, _y, _z);
	//return newVector;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::operator*=(const float& number)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->data[i][j] = this->data[i][j] * number;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
Matrix33 Matrix33::operator*(const float& number)const
{
	Matrix33 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = this->data[i][j] * number;
		}
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
Matrix33 operator*(const float& number, const Matrix33& rhs)
{
	Matrix33 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = number * rhs[i][j];
		}
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
bool Matrix33::operator==(const Matrix33& rhs)const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
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
void Matrix33::Transpose()
{
	Matrix33 temp = *this;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->data[i][j] = temp[j][i];
		}
	}
}

Matrix33 Matrix33::Transposed()
{
	Matrix33 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = this->data[j][i];
		}
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
float Matrix33::Determinant()
{
	return +this->data[0][0]*(this->data[1][1]*this->data[2][2] - this->data[1][2]*this->data[2][1])
		   -this->data[0][1]*(this->data[1][0]*this->data[2][2] - this->data[1][2]*this->data[2][0])
		   +this->data[0][2]*(this->data[1][0]*this->data[2][1] - this->data[1][1]*this->data[2][0]);
}

//------------------------------------------------------------------------------
/**
*/
Matrix33 Matrix33::Inverse() const
{
	Matrix33 result = *this;
	float det = result.Determinant();

	if (det == 0)
	{
		printf("Determinant = 0, no Inverse for this matrix. Matrix Set to Zero\n");
		result.ZeroMatrix();
		return result;
	}

	result[0][0] = (this->data[1][1]*this->data[2][2] - this->data[1][2]*this->data[2][1]);
	result[0][1] = (this->data[0][2]*this->data[2][1] - this->data[0][1]*this->data[2][2]);
	result[0][2] = (this->data[0][1]*this->data[1][2] - this->data[0][2]*this->data[1][1]);

	result[1][0] = (this->data[1][2]*this->data[2][0] - this->data[1][0]*this->data[2][2]);
	result[1][1] = (this->data[0][0]*this->data[2][2] - this->data[0][2]*this->data[2][0]);
	result[1][2] = (this->data[0][2]*this->data[1][0] - this->data[0][0]*this->data[1][2]);

	result[2][0] = (this->data[1][0]*this->data[2][1] - this->data[1][1]*this->data[2][0]);
	result[2][1] = (this->data[0][1]*this->data[2][0] - this->data[0][0]*this->data[2][1]);
	result[2][2] = (this->data[0][0]*this->data[1][1] - this->data[0][1]*this->data[1][0]);
	result = (1/det)*result;

	return result;
}

//------------------------------------------------------------------------------
/**
Calculate inertia tensor for cuboid
*/
Matrix33 Matrix33::InertiaTensor(float mass, const Vector3 &dimensions)
{
	Matrix33 I;
	I[0][0] = (1.f/12.f) * mass * (dimensions[1]*dimensions[1] + dimensions[2]*dimensions[2]);
	I[1][1] = (1.f/12.f) * mass * (dimensions[0]*dimensions[0] + dimensions[2]*dimensions[2]);
	I[2][2] = (1.f/12.f) * mass * (dimensions[0]*dimensions[0] + dimensions[1]*dimensions[1]);
	
	return I;
}

Matrix33 Matrix33::ZeroMatrix()
{
	Matrix33 zero_matrix;
	zero_matrix[0][0] = 0;
	zero_matrix[1][1] = 0;
	zero_matrix[2][2] = 0;

	return zero_matrix;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::RotateAroundX(float angle)
{
	Matrix33 result;
	result[0][0] = 1;
	result[0][1] = 0;
	result[0][2] = 0;

	result[1][0] = 0;
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][2] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);

	result[2][0] = 0;
	result[2][1] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::RotateAroundY(float angle)
{
	Matrix33 result;
	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][1] = 0;
	result[0][2] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);

	result[1][0] = 0;
	result[1][1] = 1;
	result[1][2] = 0;

	result[2][0] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = 0;
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::RotateAroundZ(float angle)
{
	Matrix33 result;
	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][1] = -sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = 0;

	result[1][0] = sinf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][2] = 0;

	result[2][0] = 0;
	result[2][1] = 0;
	result[2][2] = 1;

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::Rotate( float angle, int x, int y, int z )
{
	Matrix33 result;

	Vector3 inputVector(x, y, z);
	inputVector.Normalize();

	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[0], 2);
	result[0][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] - inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] + inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);

	result[1][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] + inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[1], 2);
	result[1][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] - inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);

	result[2][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] - inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] + inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[2], 2);

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
void Matrix33::Rotate(float angle, const Vector3 &v)
{
	Matrix33 result;

	Vector3 inputVector = v;
	inputVector.Normalize();

	result[0][0] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[0], 2);
	result[0][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] - inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[0][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] + inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);

	result[1][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[1] + inputVector[2] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[1][1] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[1], 2);
	result[1][2] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] - inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);

	result[2][0] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[0] * inputVector[2] - inputVector[1] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][1] = (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*inputVector[1] * inputVector[2] + inputVector[0] * sinf(angle*MyPersonalMathLibraryConstants::PI / 180.0f);
	result[2][2] = cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f) + (1 - cosf(angle * MyPersonalMathLibraryConstants::PI / 180.0f))*pow(inputVector[2], 2);

	*this *= result;
}

//------------------------------------------------------------------------------
/**
*/
float* Matrix33::operator[](unsigned int index)
{
	return this->data[index];
}

//------------------------------------------------------------------------------
/**
*/
const float* Matrix33::operator[](unsigned int index) const
{
	return this->data[index];
}