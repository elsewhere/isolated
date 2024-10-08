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

		m_pSquareOnlyVertices = new Mesh();
		m_pSquareOnlyVertices->generateSquare();
		m_pSquareOnlyVertices->setStreamFlags(Mesh::VERTEX_STREAM);
	}
	RenderUtils::~RenderUtils()
	{
		delete m_pSquare;
		delete m_pSquareOnlyVertices;
		delete m_pOrtho;

	}

	RenderUtils* RenderUtils::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new RenderUtils();
		}
		return sm_instance;
	}

	void RenderUtils::orthoImage(const std::string& texture, glm::vec2 pos, float scale, float alpha)
	{
		glDisable(GL_DEPTH_TEST);

		Shader& shader = g_shaders->getShader("simpletexturealpha");
		shader.bind();
		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(texture, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;
		shader.setUniform1f("alpha", alpha); GL_DEBUG;

		const float aspect = democore::g_system->getAspectRatio();

		Texture* tex = g_textures->getTexture(texture);
		float pictureaspect = (float)tex->getWidth() / (float)tex->getHeight();

		glm::mat4 modelMatrix = glm::scale(glm::vec3(scale * pictureaspect, scale * aspect, 1.f)); ;
		glm::mat4 translation = glm::translate(pos.x, pos.y, 0.f);
		modelMatrix *= translation;

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderUtils::drawBugFix()
	{
		glDisable(GL_DEPTH_TEST);

		Shader& shader = g_shaders->getShader("singlecolor");
		shader.bind();

		m_pSquareOnlyVertices->bind(&shader);


		glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f);
		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		shader.setUniform4f("color", color.x, color.y, color.z, color.a);

		const float margin = 0.002f;
		glm::mat4 modelMatrix = glm::scale(1.f, margin, 1.f);// glm::mat4(1.f);
		glm::mat4 modelMatrix2 = glm::translate(0.f, 1.f - margin, 0.f) * glm::scale(1.f, margin, 1.f);// glm::mat4(1.f);

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix2); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
	}


	void RenderUtils::fullscreenFade(glm::vec4 color)
	{
		glDisable(GL_DEPTH_TEST);

		Shader& shader = g_shaders->getShader("singlecolor");
		shader.bind();

		m_pSquareOnlyVertices->bind(&shader);

		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		shader.setUniform4f("color", color.x, color.y, color.z, color.a);
		glm::mat4 modelMatrix = glm::mat4(1.f);
		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
	}

	void RenderUtils::fullscreenQuad(const std::string& texture, Shader& shader, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(texture, GL_TEXTURE0); GL_DEBUG;

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderUtils::fullscreenQuad(Shader& shader, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void RenderUtils::fullscreenQuad(const std::string& texture, bool ignoreAspect)
	{
		glDisable(GL_DEPTH_TEST);

		const float scale = 1.f;

		Shader& shader = g_shaders->getShader("simpletexture");
		shader.bind();
		shader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(texture, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = ignoreAspect ? 1.f : democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(0.f, 0.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;

		shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
}