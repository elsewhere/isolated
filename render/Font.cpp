#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include "../glm/gtx/transform.hpp"

namespace demorender
{
	Font::Font()
	{

	}

	Font::~Font()
	{

	}

	bool Font::load(const std::string& name)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			g_debug << "Could not init freetype!" << std::endl;
			return false;
		}

		std::string fontPath = "data/fonts/" + name;
		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
		{
			g_debug << "Could not load font at path " << fontPath << std::endl;
			return false;
		}
		g_debug << "FreeType initialized, loading font " << fontPath << std::endl;
		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

		for (GLubyte i = 0; i < 128; i++)
		{
			FT_Error error = FT_Load_Char(face, i, FT_LOAD_RENDER);

			if (error)
			{
				g_debug << "Error " << error << " - could not load glyph " << i << std::endl;
				continue;
			}
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Glyph glyph;
			glyph.id = texture;
			glyph.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			glyph.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			glyph.kerning = face->glyph->advance.x;

			m_glyphs[(char)i] = glyph;
		}
		
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);



		return true;
	}

	void Font::drawString(const std::string& str, int x, int y, float scale, glm::vec3 color)
	{
		int screenWidth, screenHeight;
		democore::g_system->getWindowSize(screenWidth, screenHeight);

		glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);

		Shader&s = g_shaders->getShader("debug_text");
		s.bind();
		s.setUniform3fv("textColor", 1, (float *)&color);
		s.setUniformMatrix4fv("projection", 1, GL_FALSE, (float *)&projection);
		s.setUniform1i("text", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_VAO);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::string::const_iterator c;
		for (c = str.begin(); c != str.end(); c++)
		{
			Glyph g = m_glyphs[*c];

			GLfloat xpos = x + g.bearing.x * scale;
			GLfloat ypos = screenHeight - (y + (g.size.y - g.bearing.y) * scale) - 10;

			GLfloat w = g.size.x * scale;
			GLfloat h = g.size.y * scale;
			// Update VBO for each character
			GLfloat vertices[6][4] = 
			{
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, g.id);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (int)((g.kerning >> 6) * scale); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}