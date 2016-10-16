#pragma once

#include "node.h"
#include "shadermanager.h"
#include "debugrenderer.h"
#include <GL/glew.h>

/**
@class Light

Light Node
*/
class Light : public Node
{
public:
	/// Constructor
	Light();
	/// Destructor
	~Light();

	/// Updates the Light's world position and store it. And then call its children
	void Update(Matrix44& view, const Matrix44& model = Matrix44());
	/// Sends Light Properties(Postion, Color, Intensity, Max radius of light) to shader uniforms. And then calls the childrens' draw
	void Draw(const Matrix44& projection, const Matrix44& view);
	void DrawDebug(const Matrix44& projection, const Matrix44& view);
	/// Sets the Color of the light
	void SetLightColor(float r, float g, float b);
	/// Sets the intensity of the light
	void SetLightIntensity(float factor);

private:
	//Light position
	Vector3 light_position;
	Vector3 light_color;
	float light_intensity;
};
