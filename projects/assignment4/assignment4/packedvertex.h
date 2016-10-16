#pragma once

//Math Library
#include "mypersonalmathlib.h"
#include <string.h>

/**
@class Transform

Stores the vertex data. PackedVertex is used as a key in a map inside indexVBO function
*/
class PackedVertex
{
public:
	/// Constructor
	PackedVertex();
	/// Destructor
	~PackedVertex();

	/// Overloaded compare operator to allow this class to be used as a key in a map.
	bool operator<(const PackedVertex that) const;

	//Vertex data
	Vector3 position;
	Vector2 uv;
	Vector3 normal;

};