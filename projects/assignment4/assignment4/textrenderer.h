#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "mypersonalmathlib.h"
#include "shadermanager.h"

#include <stdio.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

/**
@class TextRenderer

text renderer
*/
class TextRenderer
{
public:
	/// Destructor
	~TextRenderer(void);
	///Static function to get the static instance
	static TextRenderer* Instance();

	void Init(const char * font_name, int font_size);
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
	void SetProjection(const Matrix44 &ortho_projection);
	void SetColor(float r, float b, float g);

private:
	//VAO
	GLuint vao;
	//VBO Vertices
	GLuint vertex_vbo;
	Matrix44 projection;

	Vector3 text_color;

	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint texture_id;   // ID handle of the glyph texture
		Vector2 size;    // Size of glyph
		Vector2 bearing;  // Offset from baseline to left/top of glyph
		GLuint advance;    // Horizontal offset to advance to next glyph
	};

	std::map<GLchar, Character> characters;

	/// Constructor
	TextRenderer();
	///Private copy constructor to prevent creating an instance other than calling the Instance() function
	TextRenderer(const TextRenderer&);
	///Private assignment constructor to prevent creating an instance other than calling the Instance() function
	TextRenderer& operator=(const TextRenderer&);
};
