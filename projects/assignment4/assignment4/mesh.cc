#include "mesh.h"

//------------------------------------------------------------------------------
/**
*/
Mesh::Mesh()
{
}

//------------------------------------------------------------------------------
/**
*/
Mesh::~Mesh(void)
{

	//Delete VBOs
	glDeleteBuffers(1, &this->vertex_vbo);
	glDeleteBuffers(1, &this->uv_vbo);
	glDeleteBuffers(1, &this->normal_vbo);
	glDeleteBuffers(1, &this->index_ebo);

	//Delete VAO
	glDeleteVertexArrays(1, &this->vao);
}

//------------------------------------------------------------------------------
/**
*/
void Mesh::Draw()
{
	//Bind this mesh VAO
	glBindVertexArray(this->vao);
	//Draw the triangles using the index buffer(EBO)
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	//Unbind the VAO
	glBindVertexArray(0);
}

//------------------------------------------------------------------------------
/**
*/
void Mesh::LoadOBJ(const char* path)
{
	//Lists to store the data read from obj file
	std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	//Lists with sorted data according the the indices specified in obj file
	std::vector<Vector3> sorted_vertices;
	std::vector<Vector2> sorted_uvs;
	std::vector<Vector3> sorted_normals;
	
	//Open the file in read only mode
	FILE* file = fopen(path, "r");
	if (file == NULL){
	 	printf("Impossible to open the file !\n");
	 	return;
	}
	 
	//Handle the data in the file
	while (true){
	 	//Read the first word of the line and store it in lineHeader
	 	char line_header[128];
	 	//Read the first word of the line as a string and store it in lineHeader
	 	int res = fscanf(file, "%s", line_header);
	 
	 	//If it's the end of the file then break the loop
	 	if (res == EOF){
	 		break; // EOF = End Of File. Quit the loop.
	 	}
	 
	 	//Read the stored data and check if it's a "v"
	 	if (strcmp(line_header, "v") == 0){ //Returns 0 if the strings are equal
	 		Vector3 vertex;
	 		//Read the data as floats and put them in the vector
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
	 		temp_vertices.push_back(vertex);
	 	}
	 	//Read the stored data and check if it's a "vt"
	 	else if (strcmp(line_header, "vt") == 0){ //Returns 0 if the strings are equal
	 		Vector2 uv;
	 		//Read the data as floats and put them in the vector
	 		fscanf(file, "%f %f\n", &uv[0], &uv[1]);
	 		//uv[1] = -uv[1]; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
	 		temp_uvs.push_back(uv);
	 	}
	 	//Read the stored data and check if it's a "vn"
	 	else if (strcmp(line_header, "vn") == 0){ //Returns 0 if the strings are equal
	 		Vector3 normal;
	 		//Read the data as floats and put them in the vector
	 		fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
	 		temp_normals.push_back(normal);
	 	}
	 	//Read the stored data and check if it's a "f"
	 	else if (strcmp(line_header, "f") == 0){ //Returns 0 if the strings are equal
	 		unsigned int vertex_index[3], uv_index[3], normal_index[3];
	 		//Read the data as integers and put them in the arrays
	 		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);
	 		//If the indices in the obj files are not 9 then we can't handle it
	 		if (matches != 9){
	 			printf("File can't be read by our simple parser :-( Try exporting with other options\n");
	 			fclose(file);
	 			return;
	 		}
	 
	 		//Add the indices of vertex data to their respective lists
	 		vertex_indices.push_back(vertex_index[0]);
	 		vertex_indices.push_back(vertex_index[1]);
	 		vertex_indices.push_back(vertex_index[2]);
	 		uv_indices.push_back(uv_index[0]);
	 		uv_indices.push_back(uv_index[1]);
	 		uv_indices.push_back(uv_index[2]);
	 		normal_indices.push_back(normal_index[0]);
	 		normal_indices.push_back(normal_index[1]);
	 		normal_indices.push_back(normal_index[2]);
	 	}
	 	else{
	 		//The data we don't need is put into this buffer just so we can move the reader forward
	 		//fgets reads the whole line. We don't want to loop multiple times if we already know the data is not something we need
	 		char stupid_buffer[1000];
	 		fgets(stupid_buffer, 1000, file);
	 	}
	}
	//Closes the file now that we are done with it
	fclose(file);
	 
	//The obj file indices specifies which 3 vertices are needed to form a triangle
	//This loop will create lists of sorted vertex data according to the obj file
	for (int i = 0; i < vertex_indices.size(); i++){
	 
	 	//Get the indices.
	 	unsigned int vertex_index = vertex_indices[i];
	 	unsigned int uv_index = uv_indices[i];
	 	unsigned int normal_index = normal_indices[i];
	 
	 	//Get the vertex data with the index extracted from the obj file
	 	//Indices start at 1 in obj files but in programming it's off by 1
	 	Vector3 vertex = temp_vertices[vertex_index - 1];
	 	Vector2 uv = temp_uvs[uv_index - 1];
	 	Vector3 normal = temp_normals[normal_index - 1];
	 
	 	//Put the vertex data in their respective lists
	 	//Will now be sorted according to how the obj file indices specifies
		sorted_vertices.push_back(vertex);
		sorted_uvs.push_back(uv);
		sorted_normals.push_back(normal);
	}
	//Vertex indexing. The lists in the class will store the indexed vertex data. Which will not have duplicates.
	//Newly generated indices list will tell which vertices are needed to form a triangle
	this->IndexVBO(sorted_vertices, sorted_uvs, sorted_normals);

	this->CreateVertexBuffers();
}

void Mesh::CreateVertexBuffers()
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
	//UV VBO
	//Create VBO on the GPU to store the vertex data
	glGenBuffers(1, &this->uv_vbo);
	//Bind VBO to make it current
	glBindBuffer(GL_ARRAY_BUFFER, this->uv_vbo);
	//Set the usage type, allocate VRAM and send the vertex data to the GPU
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(Vector2), &this->uvs[0], GL_STATIC_DRAW);

	//Sets up which shader attribute will receive the data. How many elements will form a vertex, type etc
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Enable the shader attribute to receive data
	glEnableVertexAttribArray(1);

	//////////////////////////////////////////////////////////////////////////
	//Normal VBO
	//Create VBO on the GPU to store the vertex data
	glGenBuffers(1, &this->normal_vbo);
	//Bind VBO to make it current
	glBindBuffer(GL_ARRAY_BUFFER, this->normal_vbo);
	//Set the usage type, allocate VRAM and send the vertex data to the GPU
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(Vector3), &this->normals[0], GL_STATIC_DRAW);

	//Sets up which shader attribute will receive the data. How many elements will form a vertex, type etc
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Enable the shader attribute to receive data
	glEnableVertexAttribArray(2);

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

Vector3 Mesh::CalculateDimensions()
{
	Vector3 minValues;
	Vector3 maxValues;
	
	minValues = vertices[0];
	maxValues = vertices[0];
	for(int i = 0; i < vertices.size(); ++i)
	{
		Vector3 currentVertex = vertices[i];
		maxValues[0] = std::max(maxValues[0],currentVertex[0]);
		minValues[0] = std::min(minValues[0],currentVertex[0]);
		maxValues[1] = std::max(maxValues[1],currentVertex[1]);
		minValues[1] = std::min(minValues[1],currentVertex[1]);
		maxValues[2] = std::max(maxValues[2],currentVertex[2]);
		minValues[2] = std::min(minValues[2],currentVertex[2]);		
	}
	
	Vector3 dimensions;
	dimensions[0] = maxValues[0]-minValues[0];
	dimensions[1] = maxValues[1]-minValues[1];
	dimensions[2] = maxValues[2]-minValues[2];
	
	return dimensions;
}

Vector3 Mesh::CalculateCenterOfMass()
{
	Vector3 minValues;
	Vector3 maxValues;

	minValues = vertices[0];
	maxValues = vertices[0];
	for (int i = 0; i < vertices.size(); ++i)
	{
		Vector3 currentVertex = vertices[i];
		maxValues[0] = std::max(maxValues[0], currentVertex[0]);
		minValues[0] = std::min(minValues[0], currentVertex[0]);
		maxValues[1] = std::max(maxValues[1], currentVertex[1]);
		minValues[1] = std::min(minValues[1], currentVertex[1]);
		maxValues[2] = std::max(maxValues[2], currentVertex[2]);
		minValues[2] = std::min(minValues[2], currentVertex[2]);
	}

	Vector3 dimensions;
	dimensions[0] = maxValues[0] - minValues[0];
	dimensions[1] = maxValues[1] - minValues[1];
	dimensions[2] = maxValues[2] - minValues[2];

	return minValues + dimensions/2.f;
}

AABB* Mesh::InitAABB()
{
	Vector3 minValues;
	Vector3 maxValues;

	minValues = vertices[0];
	maxValues = vertices[0];
	for (int i = 0; i < vertices.size(); ++i)
	{
		Vector3 currentVertex = vertices[i];
		maxValues[0] = std::max(maxValues[0], currentVertex[0]);
		minValues[0] = std::min(minValues[0], currentVertex[0]);
		maxValues[1] = std::max(maxValues[1], currentVertex[1]);
		minValues[1] = std::min(minValues[1], currentVertex[1]);
		maxValues[2] = std::max(maxValues[2], currentVertex[2]);
		minValues[2] = std::min(minValues[2], currentVertex[2]);
	}

	AABB* aabb = new AABB;
	aabb->min.value = minValues;
	aabb->max.value = maxValues;
	aabb->max.is_min = false;
	aabb->min.is_min = true;
	aabb->min.bounding_box = aabb;
	aabb->max.bounding_box = aabb;

	return aabb;
}

//------------------------------------------------------------------------------
/**
Arg1: Holds vertex data for one vertex
Arg2: List of vertices as keys associated with their index as value
Arg3: Stores the index of vertex found in VertexToOutIndex list
*/
bool Mesh::GetSimilarVertexIndex(PackedVertex& packed, std::map<PackedVertex, unsigned int>& VertexToOutIndex, unsigned int& result)
{
	//Iterator to find a PackedVertex in map that is equal to packed
	std::map<PackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
	//Return false if nothing is found
	if (it == VertexToOutIndex.end()){
		return false;
	}
	else{
		//Returns the value the key is paired to
		result = it->second;
		return true;
	}
}

//------------------------------------------------------------------------------
/**
 * 
Arg1: list of vertices to index
Arg2: list of uvs to index
Arg3: list of normals to index
*/
void Mesh::IndexVBO(std::vector<Vector3>& in_vertices, std::vector<Vector2>& in_uvs, std::vector<Vector3>& in_normals)
{
	//Stores unique vertex data paired with the index
	std::map<PackedVertex, unsigned int> VertexToOutIndex;

	// For each input vertex
	for (int i = 0; i < in_vertices.size(); i++){

		//Create a new PacketVertex which stores all vertex data
		PackedVertex packed;
		packed.position = in_vertices[i];
		packed.uv = in_uvs[i];
		packed.normal = in_normals[i];


		//Will store the index of the vertex found in the map
		unsigned int index;
		//Try to find a PackedVertex in the VertexToOutIndex map that is the same as packed
		//If one is found save the index of it
		bool found = GetSimilarVertexIndex(packed, VertexToOutIndex, index);

		//Push the found index to our indices vector
		if (found){ // A similar vertex is already in the VBO, use it instead !
			this->indices.push_back(index);
		}
		//If not found then add the vertex data to our vertex arrays.
		//Also add the the index of the newly added vertex data to indices array. Which will be the last element.
		//Add the vertex data to the map and associate it with the index.
		//So we can iterate next time and check for same vertex data and use the index
		else{ // If not, it needs to be added in the output data.
			this->vertices.push_back(in_vertices[i]);
			this->uvs.push_back(in_uvs[i]);
			this->normals.push_back(in_normals[i]);
			unsigned int newindex = (unsigned int)this->vertices.size() - 1;
			this->indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}