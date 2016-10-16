#pragma once

#include "mypersonalmathlib.h"
#include "packedvertex.h"
#include "debugshape.h"

//For opening files
#include <stdio.h>
#include <stdlib.h>

/**
@class DebugModel

*/
class DebugModel : public DebugShape
{
public:
	/// Constructor
	DebugModel(const char* path);
	/// Destructor
	~DebugModel();
};