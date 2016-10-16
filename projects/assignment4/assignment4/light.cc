#include "light.h"

//------------------------------------------------------------------------------
/**
*/
Light::Light()
{
	//Set default Light properties
	this->light_color.Insert(1, 1, 1);
	this->light_intensity = 1.0f;
}

Light::~Light()
{
	//Clear children list
	this->children.clear();
}

void Light::Update(Matrix44& view, const Matrix44& model)
{	
	Vector4 worldspace_light_position = model * Vector4(1,1,1,1);
	this->light_position.Insert(worldspace_light_position[0], worldspace_light_position[1], worldspace_light_position[2]);

	//Calls the childrens' update
	if (!this->children.empty())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			if (this->children[i] != nullptr)
			{
				this->children[i]->Update(view, model);
			}
		}
	}
}

void Light::Draw(const Matrix44& projection, const Matrix44& view)
{
	//Send Light properties to shader uniforms
	glUniform3fv(ShaderManager::Instance()->light_position_loc, 1, &this->light_position[0]);
	glUniform3fv(ShaderManager::Instance()->light_color_loc, 1, &this->light_color[0]);
	glUniform1f(ShaderManager::Instance()->light_intensity_loc, this->light_intensity);

	//Calls the childrens' draw
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

void Light::DrawDebug(const Matrix44& projection, const Matrix44& view)
{
	Matrix44 mvp;
	mvp[0][3] = this->light_position[0];
	mvp[1][3] = this->light_position[1];
	mvp[2][3] = this->light_position[2];

	Matrix44 debug_mvp = projection * view * mvp;
	DebugRenderer::Instance()->DrawSphere(debug_mvp);
}

void Light::SetLightColor(float r, float g, float b)
{
	this->light_color.Insert(r, g, b);
}

void Light::SetLightIntensity(float factor)
{
	this->light_intensity = factor;
}
