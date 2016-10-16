#include "debugpoint.h"

//------------------------------------------------------------------------------
/**
*/
DebugPoint::DebugPoint()
{
	this->vertices.push_back(Vector3(0.f, 0.f, 0.f));
	this->indices.push_back(0);

	this->CreateVertexBuffers();
}

//------------------------------------------------------------------------------
/**
*/
DebugPoint::~DebugPoint()
{
}