#pragma once

#include "mypersonalmathlib.h"

struct AABB;

/**
@struct SweepValue

*/
struct SweepValue
{
	AABB* bounding_box;
	Vector3 value;
	bool is_min;
};
