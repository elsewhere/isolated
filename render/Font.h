#pragma once

#include "../Globals.h"

namespace demorender
{
	class Font
	{

	public:

		Font();
		~Font();

		bool load(const std::string& name);
		void drawString(const std::string& str, int x, int y, float scale = 1.f, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0));

	protected:

		class Glyph
		{
		public:
			GLuint id;
			glm::ivec2 size;
			glm::ivec2 bearing;
			GLuint kerning;
		};

		std::map<char, Glyph> m_glyphs;

		GLuint m_VAO, m_VBO;
	};

}