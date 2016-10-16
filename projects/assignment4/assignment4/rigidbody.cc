#include "rigidbody.h"

RigidBody::RigidBody()
{
	this->mesh = nullptr;
	this->texture = nullptr;
	this->id = 0;
	
	//Set default Material properties for mesh
	this->ambient_material.Insert(0.25, 0.25, 0.25);
	this->specular_material.Insert(0.5, 0.5, 0.5);
	this->shininess = 5;
	this->picking_color.Insert(0,0,0);
	this->aabb_color.Insert(0, 0, 1);
	this->obb_color.Insert(1, 1, 0);
	this->SetID();
}

RigidBody::~RigidBody(void)
{
	this->mesh = nullptr;
	this->texture = nullptr;

	delete this->aabb;
	delete this->obb;

	//Clear children list
	this->children.clear();
}

//------------------------------------------------------------------------------
/**
First parameter is only used if the transform instance represents a camera. The view will store the inverse of the parents model matrix
The second parameter is the parent's model matrix
*/
void RigidBody::Update(Matrix44& view, const Matrix44& model)
{
	/*
	If the model matrix isn't reset and just multiplied with the parents model matrix.
	The matrices you just want to do once to place it in the world will add up and undesired effects occurs.
	For example the parent has a translation -10 and the child inherits it by matrix multiplication. 
	But the next render loop the child will have -10 and inherit additional translation from parent.
	Instead transform matrices are implemented to hold the updated transforms. Model matrix is reset and inherits these transforms + the parent's.
	*/
	this->model = this->rotation  * this->look_at * Matrix44::Translation(this->position) * this->rotation_around_own_axis * this->scaling; //A rotation is applied first to make the object rotate around itself.

	//Multiply the parent's model matrix with this
	this->model = model * this->model;
	
	//Calls the childrens' update
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Update(view, this->model);
			}
		}
	}
	
}

void RigidBody::Integrate(float timestep)
{
	if (this->is_kinematic){
		return;
	}

	Vector3 linear_acceleration = this->acceleration;
	linear_acceleration += Vector3(0.f,-9.f,0.f) + this->accum_force * this->inverse_mass;

	Vector3 angular_acceleration = inverse_inertia_tensor_world_space*this->accum_torque;

	this->velocity += linear_acceleration * timestep;
	this->angular_velocity += angular_acceleration * timestep;


	this->velocity *= pow(this->linear_damping, timestep);
	this->angular_velocity *= pow(this->angular_damping, timestep);

	//euler
	this->position += this->velocity * timestep;

	Vector3 axis = this->angular_velocity.Normalized();
	float angle = this->angular_velocity.Magnitude();
	if (angle != 0){
		Quaternion angular_velocity_q(angle * timestep, axis);
		this->orientation = (angular_velocity_q*this->orientation).Normalized();
	}

	//clear force
	this->accum_force.Insert(0, 0, 0);
	this->accum_torque.Insert(0, 0, 0);

	//calc rotation matrix
	this->rotation_around_own_axis = this->orientation.ConvertToMatrix();
}

void RigidBody::ApplyImpulse(Vector3 force, Vector3 picking_point)
{
	this->accum_force += force;

	Vector4 center_of_mass_world_space = this->model * Vector4(this->center_of_mass);

	Vector3 point = picking_point - Vector3(center_of_mass_world_space);
	Vector3 torque = Vector3::Cross(point, force);
	this->accum_torque += torque;
}

void RigidBody::DrawPass1(const Matrix44& projection, const Matrix44& view)
{
	//Form the mvp matrix from matrices supplies with draw function
	Matrix44 mvp = projection * view * this->model;
	Matrix44 model_matrix = this->model;

	//Send matrices to shader uniforms
	glUniformMatrix4fv(ShaderManager::Instance()->model_loc, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);

	glUniform1f(ShaderManager::Instance()->picking_id_loc, this->id);

	if (mesh){
		this->mesh->Draw();
	}
}

void RigidBody::Draw(const Matrix44& projection, const Matrix44& view)
{	
	//Send the material properties to shader program
	glUniform3fv(ShaderManager::Instance()->ambient_material_loc, 1, &this->ambient_material[0]);
	glUniform3fv(ShaderManager::Instance()->specular_material_loc, 1, &this->specular_material[0]);
	glUniform1f(ShaderManager::Instance()->shininess_loc, this->shininess);
	
	glUniform3fv(ShaderManager::Instance()->picking_color_loc, 1, &this->picking_color[0]);
	
	//Form the mvp matrix from matrices supplies with draw function
	Matrix44 mvp = projection * view * this->model;

	Matrix44 model_matrix = this->model;
	Matrix44 view_matrix = view;
	Matrix44 modelview_matrix = view * this->model;

	//Send matrices to shader uniforms
	glUniformMatrix4fv(ShaderManager::Instance()->model_loc, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(ShaderManager::Instance()->view_loc, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(ShaderManager::Instance()->modelview_loc, 1, GL_TRUE, &modelview_matrix[0][0]);
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);

	if(texture){
		this->texture->Bind();	
	}
	if(mesh){
		this->mesh->Draw();
	}
	
	//Calls the childrens' update
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Draw(projection, view);
			}
		}
	}
}

void RigidBody::DrawAABB(const Matrix44& projection, const Matrix44& view)
{
	if (mesh){
		Matrix44 scale;
		scale.Scale((this->aabb->max.value - this->aabb->min.value) / 2.f);	
		Vector4 center_of_mass_world_space = this->model * Vector4(this->center_of_mass);
		Matrix44 translation = Matrix44::Translation(center_of_mass_world_space);
		Matrix44 mvp = projection * view * translation * scale;

		DebugRenderer::Instance()->DrawCube(mvp, aabb_color);

	}
}

void RigidBody::DrawOBB(const Matrix44& projection, const Matrix44& view)
{
	if (mesh){
		Matrix44 scale;
		scale.Scale(this->obb->half_extent);
		Matrix44 obb_rotation = this->rotation_around_own_axis;
		Vector4 center_of_mass_world_space = this->model * Vector4(this->center_of_mass);
		Matrix44 translation = Matrix44::Translation(center_of_mass_world_space);
		Matrix44 mvp = projection * view * translation * obb_rotation * scale;

		DebugRenderer::Instance()->DrawCube(mvp, obb_color);
	}
}

void RigidBody::UpdateAABB()
{
	Matrix33 m = this->model.ConvertToMatrix33();

	Vector3 minValues = m * this->obb->vertices[0];
	Vector3 maxValues = m * this->obb->vertices[0];
	Vector3 currentVertex;

	for (int i = 0; i < 8; i++){
		currentVertex = m * this->obb->vertices[i];
		maxValues[0] = std::max(maxValues[0], currentVertex[0]);
		minValues[0] = std::min(minValues[0], currentVertex[0]);
		maxValues[1] = std::max(maxValues[1], currentVertex[1]);
		minValues[1] = std::min(minValues[1], currentVertex[1]);
		maxValues[2] = std::max(maxValues[2], currentVertex[2]);
		minValues[2] = std::min(minValues[2], currentVertex[2]);
	}

	//Added position here instead of center of mass and in render adding center of mass instead.
	//This is done because the new aabb is calculated from min/max generated from the model
	this->aabb->min.value = minValues + this->position;
	this->aabb->max.value = maxValues + this->position;
}

void RigidBody::UpdateOBB()
{
	this->obb->rot[0] = Vector3(this->model[0][0], this->model[1][0], this->model[2][0]);
	this->obb->rot[1] = Vector3(this->model[0][1], this->model[1][1], this->model[2][1]);
	this->obb->rot[2] = Vector3(this->model[0][2], this->model[1][2], this->model[2][2]);

	this->obb->pos = this->position;
}

void RigidBody::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void RigidBody::SetID()
{
	static int current_id = 0;
	this->id = ++current_id;
}

void RigidBody::SetTexture(Texture* texture)
{
	this->texture = texture;
}

int RigidBody::ID()
{
	return this->id;
}

void RigidBody::SetPickingColor(Vector3 color)
{
	this->picking_color = color;
}

Matrix44 RigidBody::GetModel()
{
	return this->model;
}

void RigidBody::Rotate(float angle, int x, int y, int z, RotateAround space)
{
	if (space == Self){ //Rotates around own axis
		this->rotation_around_own_axis.Rotate(angle, x, y, z);
	}
	else if (space == World){ //Rotate around world
		this->rotation.Rotate(angle, x, y, z);
	}
	
}

void RigidBody::Rotate(float angle, Vector3 vector, RotateAround space)
{
	if (space == Self){ //Rotates around own axis
		this->rotation_around_own_axis.Rotate(angle, vector);
	}
	else if (space == World){ //Rotate around world
		this->rotation.Rotate(angle, vector);
	}
	
}

void RigidBody::SetPosition(float x, float y, float z)
{
	this->position.Insert(x, y, z);
	//this->translation.Translate(x, y, z);
}


void RigidBody::SetPosition(Vector3 vector)
{
	this->position = vector;
	//this->translation.Translate(vector);
}

void RigidBody::Translate(float x, float y, float z)
{
	this->position += Vector3(x, y, z);
}

void RigidBody::Translate(Vector3 vector)
{
	this->position += vector;
}

void RigidBody::Scale(float factor)
{
	this->scaling.Scale(factor);
}

void RigidBody::SetScale(float factor)
{
	this->scaling.SetToIdentity();
	this->scaling.Scale(factor);
}

void RigidBody::SetInertiaTensor(const Matrix33& inertia_tensor)
{
	this->inverse_inertia_tensor = inertia_tensor;
}

void RigidBody::SetCenterOfMass(const Vector3 &center_of_mass)
{
	this->center_of_mass = center_of_mass;
}

void RigidBody::Scale(float x, float y, float z)
{
	this->scaling.Scale(x, y, z);
}

void RigidBody::Scale(Vector3 vector)
{
	this->scaling.Scale(vector);
}

//------------------------------------------------------------------------------
/**
arg1: position of object
arg2: direction of object
arg3: up vector of object
*/
void RigidBody::LookAt(Vector3 eye_position, Vector3 eye_target, Vector3 eye_up)
{
	this->look_at.LookAt(eye_position, eye_target, eye_up);

	//Invert the lookAt matrix so it can be used to move an object
	this->look_at = this->look_at.Inverse();
}

void RigidBody::SetMass(float mass)
{
	if (mass == 0){
		this->mass = 0;
		this->inverse_mass = 0;
	}
	else{
		this->mass = mass;
		this->inverse_mass = 1.f / mass;
	}
}
