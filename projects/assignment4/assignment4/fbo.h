#pragma once

#include <GL/glew.h>
#include <iostream>

/**
@class FBO

Framebuffer
*/
class FBO
{
public:
	/// Constructor
	FBO();
	/// Destructor
	~FBO();
	void Init(int width, int height);
	/// Enable Writing to FBO
	void Bind();
	void Unbind();
	//void stopreadattachment
	void UpdateTextureDimensions(int width, int height);

private:
	GLuint fbo;
	GLuint picking_texture;
	GLuint world_pos_texture;
	GLuint depth_buffer;
};

