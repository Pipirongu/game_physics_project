#include "skybox.h"

Skybox::Skybox()
{
	this->mesh = nullptr;
	this->texture = nullptr;
	
	//Set default Material properties for mesh
	this->ambient_material.Insert(0.2, 0.2, 0.2);
	this->specular_material.Insert(0.5, 0.5, 0.5);
	this->shininess = 5;
}

Skybox::~Skybox(void)
{
	//Clear children list
	this->children.clear();
}

//------------------------------------------------------------------------------
/**
First parameter is only used if the transform instance represents a camera. The view will store the inverse of the parents model matrix
The second parameter is the parent's model matrix
*/
void Skybox::Update(Matrix44& view, const Matrix44& model)
{
	/*
	If the model matrix isn't reset and just multiplied with the parents model matrix.
	The matrices you just want to do once to place it in the world will add up and undesired effects occurs.
	For example the parent has a translation -10 and the child inherits it by matrix multiplication. 
	But the next render loop the child will have -10 and inherit additional translation from parent.
	Instead transform matrices are implemented to hold the updated transforms. Model matrix is reset and inherits these transforms + the parent's.
	*/
	//Get the scale matrix
	Matrix44 skybox_model = this->rotation_around_own_axis * this->scaling ;

	//Insert the parents translation values
	skybox_model[0][3] = model[0][3];
	skybox_model[1][3] = model[1][3];
	skybox_model[2][3] = model[2][3];

	
	//Set the matrix with only scale and translation as skybox's model matrix
	this->model = skybox_model;

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

void Skybox::Draw(const Matrix44& projection, const Matrix44& view)
{
	//Form the mvp matrix from matrices supplies with draw function
	Matrix44 mvp = projection * view * this->model;
	//Send matrices to shader uniforms
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);

	if(texture){
		this->texture->BindSkybox();	
	}
	if(mesh){
		this->mesh->Draw();
	}
	
	this->texture->UnbindSkybox();	
	
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

void Skybox::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void Skybox::SetTexture(Texture* texture)
{
	this->texture = texture;
}

Matrix44 Skybox::GetModel()
{
	return this->model;
}

void Skybox::Rotate(float angle, int x, int y, int z, RotateAround space)
{
	if (space == Self){ //Rotates around own axis
		this->rotation_around_own_axis.Rotate(angle, x, y, z);
	}
	else if (space == World){ //Rotate around world
		this->rotation.Rotate(angle, x, y, z);
	}
	
}

void Skybox::Rotate(float angle, Vector3 vector, RotateAround space)
{
	if (space == Self){ //Rotates around own axis
		this->rotation_around_own_axis.Rotate(angle, vector);
	}
	else if (space == World){ //Rotate around world
		this->rotation.Rotate(angle, vector);
	}
	
}

void Skybox::Translate( float x, float y, float z )
{
	this->translation.Translate(x, y, z);
}

void Skybox::Translate( Vector3 vector )
{
	this->translation.Translate(vector);
}

void Skybox::Scale( float factor )
{
	//this->bv_scale_factor = factor;
	this->scaling.Scale(factor);
}

void Skybox::SetScale(float factor)
{
	//this->bv_scale_factor = factor;
	this->scaling.SetToIdentity();
	this->scaling.Scale(factor);
}

void Skybox::Scale( float x, float y, float z )
{
	this->scaling.Scale(x, y, z);
}

void Skybox::Scale( Vector3 vector )
{
	this->scaling.Scale(vector);
}
