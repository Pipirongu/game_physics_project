#include "packedvertex.h"

//------------------------------------------------------------------------------
/**
*/
PackedVertex::PackedVertex()
{
}

//------------------------------------------------------------------------------
/**
*/
PackedVertex::~PackedVertex()
{
}

//------------------------------------------------------------------------------
/**
*/
bool PackedVertex::operator<(const PackedVertex that) const
{
	return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) < 0;
};