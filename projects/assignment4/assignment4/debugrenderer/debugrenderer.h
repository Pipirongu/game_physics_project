#pragma once

#include "debugmodel.h"
#include "debugpoint.h"
#include "debugline.h"
#include "shadermanager.h"
#include <map>

/**
@class DebugShapeManager

*/
class DebugRenderer
{
public:
	/// Destructor
	~DebugRenderer();
	
	///Static function to get the static instance
	static DebugRenderer* Instance();

	void AddDebugShape(const char* path, std::string name);

	void Draw(std::string name_of_debug_shape, const Matrix44& mvp, const Vector3& color = Vector3(1.f,1.f,1.f));
	void DrawCube(const Matrix44& mvp, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	void DrawSphere(const Matrix44& mvp, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	void DrawPoint(const Matrix44& mvp, float size = 5, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	void DrawLine(const Matrix44& view, const Matrix44& projection, const Vector3& normal, const Vector3& position, float width = 5, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	void DrawPlaneN(const Matrix44& view, const Matrix44& projection, const Vector3& normal, const Vector3& position, const Vector3& color, const Vector3& halfExtent = Vector3(1, 1, 1));
	//Matrix44* view;
	//Matrix44* projection;

private:
	//map with shaders
	std::map<std::string, DebugShape*> debug_shapes;

	/// Constructor
	DebugRenderer();
	///Private copy constructor to prevent creating an instance other than calling the Instance() function
	DebugRenderer(const DebugRenderer&);
	///Private assignment constructor to prevent creating an instance other than calling the Instance() function
	DebugRenderer& operator=(const DebugRenderer&);
};