#pragma once

#include "mypersonalmathlib.h"

class RigidBody;

/**
@struct OBB

*/
struct OBB
{
	~OBB()
	{
		object = nullptr;
	}

	Vector3 rot[3];
	Vector3 pos;
	Vector3 half_extent;

	Vector3 vertices[8];

	RigidBody* object;

	inline void InitVertexList(Vector3 min, Vector3 max)
	{
		this->vertices[0] = Vector3(max[0], max[1], max[2]);
		this->vertices[1] = Vector3(min[0], max[1], max[2]);
		this->vertices[2] = Vector3(min[0], min[1], max[2]);
		this->vertices[3] = Vector3(max[0], min[1], max[2]);
		this->vertices[4] = Vector3(min[0], min[1], min[2]);
		this->vertices[5] = Vector3(max[0], min[1], min[2]);
		this->vertices[6] = Vector3(max[0], max[1], min[2]);
		this->vertices[7] = Vector3(min[0], max[1], min[2]);
	}
};
