#pragma once

#include "node.h"
#include <GLFW/glfw3.h>

/**
@class Camera

Camera class
*/
class Camera : public Node
{
public:
	/// Used by Rotate functions to decide how the object should rotate
	enum RotateAround
	{
		Self,
		World
	};
	
	/// Constructor
	Camera();
	/// Destructor
	~Camera(void);

	/// Updates the model/view matrix and then calls its childrens' update
	void Update(Matrix44& view, const Matrix44& model = Matrix44());
	/// Returns the model or view matrix
	Matrix44 GetView();

	/// inverted camera lookat function for objects
	void UpdateCameraMatrix();
	void MouseInput(double mouse_pos_x, double mouse_pos_y, int width, int height, float delta_time);
	void KeyboardInput(int up, int down, int left, int right, float delta_time);
	void UpdateCamera();

	//Vectors for camera control
	Vector3 position; //Stores the position
	Vector3 direction; //Stores the direction
	Vector3 right; //The right vector
	Vector3 up; //Up vector will be calculated using direction and right vectors

private:
	Matrix44 view;

	//A Combination of Translation/Rotation matrices
	Matrix44 look_at;

	//Horizontal Angle( left and right ) 
	float horizontal_angle;
	//Vertical Angle( left and right ) 
	float vertical_angle;
	//Sensitivity of keyboard input 
	float keyboard_sensitivity;
	//Sensitivity of mouse input
	float mouse_sensitivity;
};
