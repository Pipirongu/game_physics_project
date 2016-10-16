#include "camera.h"

//------------------------------------------------------------------------------
/**
*/
Camera::Camera()
{
	//Start position of the player in world space
	this->position.Insert(0, 0, 4);

	// horizontal angle : toward -Z
	this->horizontal_angle = 3.14f;
	// vertical angle : 0, look at the horizon
	this->vertical_angle = 0.0f;

	this->keyboard_sensitivity = 10.0f;
	this->mouse_sensitivity = 0.005f;
}

//------------------------------------------------------------------------------
/**
*/
Camera::~Camera(void)
{
	//Clear children list
	this->children.clear();
}

//------------------------------------------------------------------------------
/**
arg1 out matrix to store view matrix
arg2: parent's model matrix
*/
void Camera::Update(Matrix44& view, const Matrix44& model)
{
	/*
	If the model matrix isn't reset and just multiplied with the parents model matrix.
	The matrices you just want to do once to place it in the world will add up and undesired effects occurs.
	For example the parent has a translation -10 and the child inherits it by matrix multiplication. 
	But the next render loop the child will have -10 and inherit additional translation from parent.
	Instead transform matrices are implemented to hold the updated transforms. Model matrix is reset and inherits these transforms + the parent's.
	*/
	this->view = this->look_at; //A rotation is applied first to make the object rotate around itself.

	//Multiply the parent's model matrix with this
	this->view = model * this->view;
	
	//View Matrix
	//Invert the player's model matrix and set it as the view matrix
	view = this->view.Inverse();

	//Calls the childrens' update
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Update(view, this->view);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
Matrix44 Camera::GetView()
{
	return this->view;
}

//------------------------------------------------------------------------------
/**
arg1: position of object
arg2: direction of object
arg3: up vector of object
*/
void Camera::UpdateCameraMatrix()
{
	this->look_at.LookAt(this->position, this->position + this->direction, this->up);

	//Invert the lookAt matrix so it can be used to move an object
	this->look_at = this->look_at.Inverse();
}

void Camera::MouseInput(double mouse_pos_x, double mouse_pos_y, int width, int height, float delta_time)
{
	double mid_x = width / 2;
	double mid_y = height / 2;

	//////////////////////////////////////////////////////////////////////////
	// Compute new orientation
	//look left and right
	this->horizontal_angle += this->mouse_sensitivity * float(mid_x - mouse_pos_x);
	//look up and down
	this->vertical_angle += this->mouse_sensitivity * float(mid_y - mouse_pos_y);

	//Locks the vertical angle to -90 to 90
	if (this->vertical_angle*180.0 / MyPersonalMathLibraryConstants::PI > 90)
		this->vertical_angle = 90 * MyPersonalMathLibraryConstants::PI / 180.0;
	if (this->vertical_angle*180.0 / MyPersonalMathLibraryConstants::PI < -90)
		this->vertical_angle = -90 * MyPersonalMathLibraryConstants::PI / 180.0;

	//Sets the horizontal angle between 0 and 360 
	if (this->horizontal_angle*180.0 / MyPersonalMathLibraryConstants::PI < 0.0)
		this->horizontal_angle += 360.0*MyPersonalMathLibraryConstants::PI / 180.0;
	if (this->horizontal_angle*180.0 / MyPersonalMathLibraryConstants::PI > 360.0)
		this->horizontal_angle -= 360 * MyPersonalMathLibraryConstants::PI / 180.0;

	//Update the camera vectors
	this->UpdateCamera();
}

void Camera::KeyboardInput(int up, int down, int left, int right, float delta_time)
{
	if (up == GLFW_PRESS){
		this->position += this->direction * delta_time * this->keyboard_sensitivity;
	}
	if (down == GLFW_PRESS){
		this->position -= this->direction * delta_time  * this->keyboard_sensitivity;
	}
	if (left == GLFW_PRESS){
		this->position -= this->right * delta_time  * this->keyboard_sensitivity;
	}
	if (right== GLFW_PRESS){
		this->position += this->right * delta_time  * this->keyboard_sensitivity;
	}
}

void Camera::UpdateCamera()
{
	//Calculate the new direction vector
	this->direction.Insert(
		cos(this->vertical_angle) * sin(this->horizontal_angle),
		sin(this->vertical_angle),
		cos(this->vertical_angle) * cos(this->horizontal_angle));

	//Calculate the new right vector
	this->right.Insert(
		sin(this->horizontal_angle - 3.14f / 2.0f),
		0,
		cos(this->horizontal_angle - 3.14f / 2.0f));

	//Cross product to find the new up vector
	this->up = Vector3::Cross(this->right, this->direction);
}
