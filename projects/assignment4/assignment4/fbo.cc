#include "fbo.h"

FBO::FBO()
{
}


FBO::~FBO()
{
	glDeleteFramebuffers(1, &this->fbo);
	glDeleteRenderbuffers(1, &this->depth_buffer);
	glDeleteTextures(1, &this->picking_texture);
	glDeleteTextures(1, &this->world_pos_texture);
}

void FBO::Init(int width, int height)
{
	//////////////////////////////////////////////////////////////////////////
	// Creating the blur FBO
	glGenFramebuffers(1, &this->fbo);
	//Bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	
	// Bind the depth buffer
	glGenRenderbuffers(1, &this->depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_buffer);
	
	
	glGenTextures(1, &this->picking_texture);
	glBindTexture(GL_TEXTURE_2D, this->picking_texture);
	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_R32F, this->window_width, this->window_height, GL_FALSE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
	//Attach Textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->picking_texture, 0);

	
	glGenTextures(1, &this->world_pos_texture);
	glBindTexture(GL_TEXTURE_2D, this->world_pos_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->world_pos_texture, 0); 

	// Disable reading to avoid problems with older GPUs
	glReadBuffer(GL_NONE);

	
	//Check if framebuffer is OK
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE){
		std::cout<<"FB error, status: 0x"<<fboStatus<<std::endl;
		return;
	}

	GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FBO::UpdateTextureDimensions(int width, int height)
{
	glBindRenderbuffer(GL_RENDERBUFFER, this->depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
	
	glBindTexture(GL_TEXTURE_2D, this->picking_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
	
	glBindTexture(GL_TEXTURE_2D, this->world_pos_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
}