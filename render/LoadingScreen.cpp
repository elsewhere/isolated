#include "../Globals.h"
#include "LoadingScreen.h"
#include "../glm/gtx/transform.hpp"
#include "Camera.h"

using namespace demomath;

namespace demorender
{
	LoadingScreen::LoadingScreen()
	{
		m_pOrtho = std::make_unique<OrthoCamera>(0.f, 1.f, 0.f, 1.f);
		m_pSquare = std::make_unique<Model>();

		Mesh* mesh = new Mesh();
		mesh->generateSquare();
		mesh->setStreamFlags(Mesh::VERTEX_STREAM);

		m_pSquare->setMesh(mesh);

		m_shader = ShaderManager::createShaderFromFile("data\\shaders\\loadingscreen\\singlecolor.shader");
		if (!m_shader)
		{
			g_debug << "could not create loading screen shader\n";
		}
		else
		{
			g_debug << "init loading screen\n";
		}
	}

	LoadingScreen::~LoadingScreen()
	{

	}

	void LoadingScreen::drawRectangle(float startX, float startY, float endX, float endY, glm::vec3 color)
	{
		m_shader->bind();
		m_shader->setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(m_shader);

		m_shader->setUniform3f("color", color.x, color.y, color.z);

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 modelMatrix = glm::translate(startX, startY, 0.f);

		glm::mat4 scaling = glm::scale(glm::vec3((endX - startX), (endY - startY), 1.f));
		modelMatrix *= scaling;
		m_shader->setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
	}

	void LoadingScreen::draw(float percentage)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		int width, height;
		democore::g_system->getWindowSize(width, height);

		const float aspect = (float)width / (float)height;

		const float MARGIN_X = 0.35f;
		const float HEIGHT = 0.005f;
		const float BORDER = 0.003f;
		const float BORDER2 = 0.005f;

		const float BORDER_Y = BORDER * aspect;
		const float BORDER_Y2 = BORDER2 * aspect;

		drawRectangle(MARGIN_X - BORDER2, 0.5f - HEIGHT * 0.5f - BORDER_Y2, 1.f - MARGIN_X + BORDER2, 0.5f + HEIGHT * 0.5f + BORDER_Y2, glm::vec3(0.25f)); //background
		drawRectangle(MARGIN_X - BORDER, 0.5f - HEIGHT * 0.5f - BORDER_Y, 1.f - MARGIN_X + BORDER, 0.5f + HEIGHT * 0.5f + BORDER_Y, glm::vec3(0.f)); //empty
		drawRectangle(MARGIN_X, 0.5f - HEIGHT * 0.5f, MARGIN_X + percentage * (1.f - 2 * MARGIN_X), 0.5f + HEIGHT * 0.5f, glm::vec3(0.7f)); //bar
	}
}