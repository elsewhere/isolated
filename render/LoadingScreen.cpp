#include "../Globals.h"
#include "LoadingScreen.h"
#include "../glm/gtx/transform.hpp"
#include "Camera.h"

using namespace demomath;

namespace demorender
{
	LoadingScreen::LoadingScreen()
	{
		int width, height;
		democore::g_system->getWindowSize(width, height);

		m_pOrtho = std::make_unique<OrthoCamera>(0.f, 1024.f, 0.f, 768.f);
		m_pSquare = std::make_unique<Model>();

		Mesh* mesh = new Mesh();
		mesh->generateSquare();
		mesh->setStreamFlags(Mesh::VERTEX_STREAM | Mesh::UV_STREAM);

		m_pSquare->setMesh(mesh);

		m_shader = ShaderManager::createShaderFromFile("data\\shaders\\singlecolor.shader");
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

	void LoadingScreen::draw(float percentage)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		m_shader->bind();
		m_shader->setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(m_shader);

		m_shader->setUniform3f("color", 1.f, 1.f, 1.f);
	
		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(256.f, 256.f, 0.f);
		const float scale = 1.f;

		glm::mat4 scaling = glm::scale(glm::vec3(percentage * 768.f, 50.f, 1.f));
		testiModelMatrix *= scaling;
		m_shader->setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}