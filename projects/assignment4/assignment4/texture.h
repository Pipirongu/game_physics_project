#pragma once

#include <GL/glew.h>
#include <string>
#include "shadermanager.h"
#include "stb_image.h"
/**
@class Texture

Holds the texture of a mesh
*/
class Texture
{
public:
	/// Constructor
	Texture();
	/// Destructor
	~Texture();

	/// Bind Texture
	void Bind();
	void BindSkybox();
	void Unbind();
	void UnbindSkybox();
	/// Loads a texture to be used by a mesh
	void LoadTexture(const std::string& file_name);
	/// Loads 6 images for cube map meant to be used by a skybox mesh
	void LoadSkyboxTexture(const std::string& back, const std::string& down, const std::string& front, const std::string& left, const std::string& right, const std::string& up);

private:
	//Holds the texture that should be used by this mesh
	GLuint texture_id;
};

