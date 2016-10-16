#pragma once

#include "mypersonalmathlib.h"
#include "sweepvalue.h"

class RigidBody;

/**
@struct AABB

*/
struct AABB
{
	~AABB()
	{
		object = nullptr;
	}

	SweepValue min;
	SweepValue max;

	RigidBody* object;
};
