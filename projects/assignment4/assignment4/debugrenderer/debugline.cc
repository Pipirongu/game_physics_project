#include "debugline.h"

//------------------------------------------------------------------------------
/**
*/
DebugLine::DebugLine()
{
	this->vertices.push_back(Vector3(0.f, 0.f, 0.f));
	this->vertices.push_back(Vector3(0.f, 0.f, 1.f));
	this->indices.push_back(0);
	this->indices.push_back(1);

	this->CreateVertexBuffers();
}

//------------------------------------------------------------------------------
/**
*/
DebugLine::~DebugLine()
{
}