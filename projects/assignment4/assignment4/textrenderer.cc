#include "textrenderer.h"

TextRenderer::TextRenderer()
{
	this->text_color.Insert(1.f, 1.f, 1.f);
}


TextRenderer::~TextRenderer()
{
	// Delete buffers
	glDeleteBuffers(1, &this->vao);
	glDeleteBuffers(1, &this->vertex_vbo);

	for (std::map<GLchar, Character>::iterator it = this->characters.begin(); it != this->characters.end(); it++){
		glDeleteTextures(1, &it->second.texture_id);
	}
	this->characters.clear();
}

//------------------------------------------------------------------------------
/**
*/
TextRenderer* TextRenderer::Instance()
{
	static TextRenderer instance;

	return &instance;
}

void TextRenderer::Init(const char *font_name, int font_size)
{
	// FreeType
	FT_Library library;
	FT_Face face;

	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&library)){
		std::cerr << "Couldn't initialize FreeType library\n";
	}

	// Load font as face
	if (FT_New_Face(library, font_name, 0, &face)){
		std::cerr << "Couldn't initialize FreeType library\n";
	}

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, font_size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "ERROR::FREETYTPE: Failed to load Glyph\n";
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		this->characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
	glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//send the ortho projection here 
	glUniformMatrix4fv(ShaderManager::Instance()->projection_loc, 1, GL_TRUE, &this->projection[0][0]);
	glUniform3fv(ShaderManager::Instance()->text_color_loc, 1, &this->text_color[0]);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ShaderManager::Instance()->texture_sampler_loc, 0); 
	glBindVertexArray(this->vao);


	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = this->characters[*c];

		GLfloat xpos = x + ch.bearing[0] * scale;
		GLfloat ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

		GLfloat w = ch.size[0] * scale;
		GLfloat h = ch.size[1] * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void TextRenderer::SetProjection(const Matrix44 &ortho_projection)
{
	this->projection = ortho_projection;
}

void TextRenderer::SetColor(float r, float b, float g)
{
	this->text_color.Insert(r, g, b);
}
