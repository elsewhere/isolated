#pragma once

#include "../Globals.h"

namespace demorender
{
	
	class Shader;

	class LoadingScreen
	{
	public:
		LoadingScreen();
		~LoadingScreen();
		void draw(float percentage); 

	private:

		std::unique_ptr<class OrthoCamera> m_pOrtho;
		std::unique_ptr<class Model> m_pSquare;

		void drawRectangle(float startX, float startY, float endX, float endY, glm::vec3 color);
		Shader* m_shader;

	};
}