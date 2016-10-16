//------------------------------------------------------------------------------
// main.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "renderscene.h"

int main(int argc, const char** argv)
{
	if (Graphics::RenderScene::Instance()->Open())
	{
		Graphics::RenderScene::Instance()->Run();
		Graphics::RenderScene::Instance()->Close();
	}
	Graphics::RenderScene::Instance()->Exit();
	
}