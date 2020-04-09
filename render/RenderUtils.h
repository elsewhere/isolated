#pragma once

#include "../Globals.h"

#define g_renderUtils RenderUtils::inst()

namespace demorender
{
	class RenderUtils
	{
	public:

		static RenderUtils* inst();

		void fullscreenQuad(const std::string& texture, bool ignoreAspect = true);
		void fullscreenQuad(const std::string& texture, Shader& shader, bool ignoreAspect = true);
		void fullscreenQuad(Shader& shader, bool ignoreAspect = true);
		void fullscreenFade(glm::vec4 color);

		void orthoImage(const std::string& texture, glm::vec2 pos, float scale, float alpha);

		void drawBugFix();

	private:
		RenderUtils();
		~RenderUtils();

		static RenderUtils* sm_instance;
		class OrthoCamera* m_pOrtho;
		class Mesh *m_pSquare;
		class Mesh *m_pSquareOnlyVertices;

	};
}