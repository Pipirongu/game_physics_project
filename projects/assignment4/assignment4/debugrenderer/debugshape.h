#pragma once

#include <vector>
#include <GL/glew.h>
#include "mypersonalmathlib.h"
#include "packedvertex.h"
#include <map>

//For opening files
#include <stdio.h>
#include <stdlib.h>

/**
@class DebugModel

*/
class DebugShape
{
public:
	/// Constructor
	DebugShape();
	/// Destructor
	virtual ~DebugShape() = 0;

	//vertex list
	std::vector<Vector3> vertices;
	std::vector<unsigned int> indices;

	//VAO
	GLuint vao;
	//VBO Vertices
	GLuint vertex_vbo;
	//EBO Indices
	GLuint index_ebo;

protected:
	virtual void CreateVertexBuffers();
};