#include "debugmodel.h"

//------------------------------------------------------------------------------
/**
*/
DebugModel::DebugModel(const char* path)
{
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
			this->vertices.push_back(vertex);
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
			this->indices.push_back(vertex_index[0]);
			this->indices.push_back(vertex_index[1]);
			this->indices.push_back(vertex_index[2]);
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

	this->CreateVertexBuffers();
}

//------------------------------------------------------------------------------
/**
*/
DebugModel::~DebugModel()
{
}