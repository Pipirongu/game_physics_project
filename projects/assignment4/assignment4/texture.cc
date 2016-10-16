#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

Texture::Texture()
{
}


Texture::~Texture()
{
	glDeleteTextures(1, &this->texture_id);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	glUniform1i(ShaderManager::Instance()->texture_sampler_loc, 0);
}

void Texture::BindSkybox()
{
	//Cubemap for skybox
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);
	glUniform1i(ShaderManager::Instance()->texture_sampler_loc, 0);

	//We are inside the skybox so we cull the front face instead
	glCullFace(GL_FRONT);
	//Too make the skybox a part of the scene we need to change the depth comparison to LESS or Equal.
	//Because we set the z value in shader to w to make sure it will be 1 after perspective division
	glDepthFunc(GL_LEQUAL);
}

void Texture::Unbind()
{
	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UnbindSkybox()
{
	//Unbind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

}
void Texture::LoadTexture(const std::string& file_name)
{
	int w, h, n;
	unsigned char* data = stbi_load(file_name.c_str(), &w, &h, &n, 0);
	//let QT guess the file format of image and convert the image to a format openGL can handle
	//QImage texture = QGLWidget::convertToGLFormat(QImage(texturepath));

	//Create openGL Texture
	glGenTextures(1, &this->texture_id);
	//Bind it
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	//Fill with data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);
	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

//------------------------------------------------------------------------------
/**
 * 
Arg1: Path of the back(-Z) texture
Arg2: Path of the down(-Y) texture
Arg3: Path of the front(+Z) texture
Arg4: Path of the left(-X) texture
Arg5: Path of the right(+X) texture
Arg6: Path of the up(+Y) texture
*/
void Texture::LoadSkyboxTexture(const std::string& back, const std::string& down, const std::string& front, const std::string& left, const std::string& right, const std::string& up)
{
	int back_w, back_y, back_n;
	int down_w, down_y, down_n;
	int front_w, front_y, front_n;
	int left_w, left_y, left_n;
	int right_w, right_y, right_n;
	int up_w, up_y, up_n;
	
	unsigned char* back_texture = stbi_load(back.c_str(), &back_w, &back_y, &back_n, 0);
	unsigned char* down_texture = stbi_load(down.c_str(), &down_w, &down_y, &down_n, 0);
	unsigned char* front_texture = stbi_load(front.c_str(), &front_w, &front_y, &front_n, 0);
	unsigned char* left_texture = stbi_load(left.c_str(), &left_w, &left_y, &left_n, 0);
	unsigned char* right_texture = stbi_load(right.c_str(), &right_w, &right_y, &right_n, 0);
	unsigned char* up_texture = stbi_load(up.c_str(), &up_w, &up_y, &up_n, 0);

	//Create openGL Texture
	glGenTextures(1, &this->texture_id);
	//Bind it
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);

		
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right_w, right_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, right_texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, down_w, down_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, down_texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front_w, front_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, front_texture);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left_w, left_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, left_texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, up_w, up_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, up_texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back_w, back_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, back_texture);
		

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	//Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	stbi_image_free(back_texture);
	stbi_image_free(down_texture);
	stbi_image_free(front_texture);
	stbi_image_free(left_texture);
	stbi_image_free(right_texture);
	stbi_image_free(up_texture);
}