#pragma once

//For class representing key for map in the vertex indexing algorithm
#include "packedvertex.h"
#include <map>

#include <GL/glew.h>

//For opening files
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include "aabb.h"

/**
@class Mesh

Represents the mesh of an object. Can load obj files.
*/
class Mesh
{
public:
	/// Constructor
	Mesh();
	/// Destructor
	~Mesh();

	/// Draw the mesh, and then calls the childrens' draw
	void Draw();
	/// Object Loader. Takes the path to the obj file. Reads the data and puts it into VBOs on the VRAM
	void LoadOBJ(const char* path);
	Vector3 CalculateDimensions();
	Vector3 CalculateCenterOfMass();
	AABB* InitAABB();
	
	//vertices, uvs, normals, indices
	std::vector<Vector3> vertices;
	std::vector<Vector2> uvs;
	std::vector<Vector3> normals;
	std::vector<unsigned int> indices;

private:
	
	//VAO
	GLuint vao;
	//VBO Vertices
	GLuint vertex_vbo;
	//VBO UVS
	GLuint uv_vbo;
	//VBO Normals
	GLuint normal_vbo;
	//EBO Indices
	GLuint index_ebo;

	void CreateVertexBuffers();
	/// Used by indexVBO function to compare a vertex with vertices in a list. If one is found in the list. Function returns true and result parameter will hold the index of the vertex found in the list
	bool GetSimilarVertexIndex(PackedVertex& packed, std::map<PackedVertex, unsigned int>& VertexToOutIndex, unsigned int& result);
	/// Vertex indexing. Generates lists of vertices with no duplicates. An indices list is also generated to tell which vertices should form a triangle.
	void IndexVBO(std::vector<Vector3>& in_vertices, std::vector<Vector2>& in_uvs, std::vector<Vector3>& in_normals);
};
