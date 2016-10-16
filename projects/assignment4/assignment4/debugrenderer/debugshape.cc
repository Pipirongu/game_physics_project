#include "debugshape.h"

//------------------------------------------------------------------------------
/**
*/
DebugShape::DebugShape()
{
}

//------------------------------------------------------------------------------
/**
*/
DebugShape::~DebugShape()
{
	glDeleteBuffers(1, &this->vertex_vbo);
	glDeleteBuffers(1, &this->index_ebo);

	//Delete VAO
	glDeleteVertexArrays(1, &this->vao);
}

void DebugShape::CreateVertexBuffers()
{
	//////////////////////////////////////////////////////////////////////////
	//Create VAO
	glGenVertexArrays(1, &this->vao);
	//Bind VAO
	glBindVertexArray(this->vao);

	//////////////////////////////////////////////////////////////////////////
	//Vertex VBO
	//Create VBO on the GPU to store the vertex data
	glGenBuffers(1, &this->vertex_vbo);
	//Bind VBO to make it current
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_vbo);
	//Set the usage type, allocate VRAM and send the vertex data to the GPU
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vector3), &this->vertices[0], GL_STATIC_DRAW);

	//Sets up which shader attribute will received the data. How many elements will form a vertex, type etc
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Enable the shader attribute to receive data
	glEnableVertexAttribArray(0);

	//////////////////////////////////////////////////////////////////////////
	//Indices EBO
	//Create EBO on the GPU to store the vertex data
	glGenBuffers(1, &this->index_ebo);
	//Bind EBO to make it current
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_ebo);
	//Set the usage type, allocate VRAM and send the vertex data to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	//////////////////////////////////////////////////////////////////////////
	//Unbind the VAO now that the VBOs have been set up
	glBindVertexArray(0);
}