#include "../Globals.h"
#include "RenderUtils.h"
#include "../glm/gtx/transform.hpp"
#include "Camera.h"

using namespace demomath;

namespace demorender
{
	RenderUtils* RenderUtils::sm_instance = nullptr;

	RenderUtils::RenderUtils():m_pOrtho(0),
		m_pSquare(0)
	{
		int width, height;
		democore::g_system->getWindowSize(width, height);
		
		m_pOrtho = new OrthoCamera(0.f, 1.f, 0.f, 1.f);

		m_pSquare = new Mesh();
		m_pSquare->generateSquare();
		m_pSquare->setStreamFlags(Mesh::VERTEX_STREAM | Mesh::UV_STREAM);
	}
	RenderUtils::~RenderUtils()
	{

	}

	RenderUtils* RenderUtils::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new RenderUtils();
		}
		return sm_instance;
	}

	void RenderUtils::fullscreenQuad(const std::string& texture, Shader& shader, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(texture, GL_TEXTURE0); GL_DEBUG;

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);

		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderUtils::fullscreenQuad(Shader& shader, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);

		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}


	void RenderUtils::fullscreenQuad(const std::string& texture, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		const float scale = 1.f;
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("simpletexture");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(texture, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;

		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

/*
	void RenderDebug::drawTexture(const std::string& name, int x, int y, float scale)
	{
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("simpletexture");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(name, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(x * 1.f, y * 1.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;
		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	*/
}