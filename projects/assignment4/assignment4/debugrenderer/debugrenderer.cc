#include "debugrenderer.h"

//------------------------------------------------------------------------------
/**
*/
DebugRenderer::DebugRenderer()
{
	this->AddDebugShape("models/wireframe_cube.obj", "cube");
	this->AddDebugShape("models/sphere.obj", "sphere");
	this->debug_shapes["point"] = new DebugPoint;
	this->debug_shapes["line"] = new DebugLine;
}

//------------------------------------------------------------------------------
/**
*/
DebugRenderer::~DebugRenderer()
{
	for (std::map<std::string, DebugShape*>::iterator it = this->debug_shapes.begin(); it != this->debug_shapes.end(); it++){
		delete it->second;
	}
	this->debug_shapes.clear();
}

//------------------------------------------------------------------------------
/**
*/
DebugRenderer* DebugRenderer::Instance()
{
	static DebugRenderer instance;

	return &instance;
}

void DebugRenderer::AddDebugShape(const char* path, std::string name)
{
	if (this->debug_shapes.count(name) <= 0){
		this->debug_shapes[name] = new DebugModel(path);
	}
	else{
		printf("Debug shape name already exists..");
	}
}

void DebugRenderer::Draw(std::string name_of_debug_shape, const Matrix44& mvp, const Vector3& color)
{
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugModel* debug_shape = (DebugModel*)this->debug_shapes[name_of_debug_shape];
	glBindVertexArray(debug_shape->vao);

	//size of debug vao
	glDrawElements(GL_TRIANGLES, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);

	//Unbind the VAO
	glBindVertexArray(0);
}

void DebugRenderer::DrawCube(const Matrix44& mvp, const Vector3& color)
{
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugShape* debug_shape = this->debug_shapes["cube"];
	glBindVertexArray(debug_shape->vao);

	glLineWidth(2);
	//size of debug vao
	glDrawElements(GL_LINES, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.f);

	//Unbind the VAO
	glBindVertexArray(0);
}

void DebugRenderer::DrawSphere(const Matrix44& mvp, const Vector3& color)
{
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugShape* debug_shape = this->debug_shapes["sphere"];
	glBindVertexArray(debug_shape->vao);

	//size of debug vao
	glDrawElements(GL_TRIANGLES, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);

	//Unbind the VAO
	glBindVertexArray(0);
}

void DebugRenderer::DrawPoint(const Matrix44& mvp, float size, const Vector3& color)
{
	ShaderManager::Instance()->ChangeShader("debug");
	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugPoint* debug_shape = (DebugPoint*)this->debug_shapes["point"];
	glBindVertexArray(debug_shape->vao);

	glPointSize(size);
	glDrawElements(GL_POINTS, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);

	//Unbind the VAO
	glBindVertexArray(0);
	ShaderManager::Instance()->ChangeShader("main");
}

void DebugRenderer::DrawLine(const Matrix44& view, const Matrix44& projection, const Vector3& normal, const Vector3& position, float width, const Vector3& color)
{
	ShaderManager::Instance()->ChangeShader("debug");
	Matrix44 model = Matrix44::Translation(position);
	Vector3 axis = Vector3::Cross(Vector3(0, 0, 1), normal);
	float tetha = acos(normal.z);
	if (axis.SquareMagnitude() < 0.0001f)
	{
		axis = Vector3(1, 0, 0);
	}
	if (tetha != 0)
	{
		float  deg = (tetha * 180) / 3.14159f;
		Matrix44 test;
		test.Rotate(deg, axis);
		model = model*test;
	}

	Matrix44 mvp = projection * view * model;

	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugLine* debug_shape = (DebugLine*)this->debug_shapes["line"];
	glBindVertexArray(debug_shape->vao);

	glLineWidth(width);
	glDrawElements(GL_LINES, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.f);

	//Unbind the VAO
	glBindVertexArray(0);
	ShaderManager::Instance()->ChangeShader("main");
}

void DebugRenderer::DrawPlaneN(const Matrix44& view, const Matrix44& projection, const Vector3& normal, const Vector3& position, const Vector3& color, const Vector3& halfExtent/*= Vector3(1, 1, 1)*/)
{
	ShaderManager::Instance()->ChangeShader("debug");
	Matrix44 model = Matrix44::Translation(position);
	Vector3 axis = Vector3::Cross(Vector3(0, 0, 1),normal);
	float tetha = acos(normal.z);
	if (axis.SquareMagnitude() < 0.0001f)
	{
		axis = Vector3(1, 0, 0);
	}
	if (tetha != 0)
	{
		float  deg = (tetha * 180) / 3.14159f;
		Matrix44 test;
		test.Rotate(deg, axis);
		model = model*test;
	}
	Matrix44 scale;
	//scale.Scale(halfExtent);
	model = model * scale;
	Matrix44 mvp = projection * view * model;

	glUniformMatrix4fv(ShaderManager::Instance()->mvp_loc, 1, GL_TRUE, &mvp[0][0]);
	glUniform3fv(ShaderManager::Instance()->wireframe_color_loc, 1, &color[0]);

	DebugLine* debug_shape = (DebugLine*)this->debug_shapes["line"];
	glBindVertexArray(debug_shape->vao);

	//glLineWidth(width);
	glDrawElements(GL_LINES, debug_shape->indices.size(), GL_UNSIGNED_INT, 0);
	//glLineWidth(1.f);

	//Unbind the VAO
	glBindVertexArray(0);
	ShaderManager::Instance()->ChangeShader("main");
}
