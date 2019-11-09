#include "PostProcessTesti2.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

namespace
{
	static const std::string skyboxTexture = "miramar";
}

void PostProcessTesti2::init()
{
	m_pModel  = new demorender::Model();
	m_pModel->setMesh("asteroid");

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");

	m_camera = new demorender::Camera(1.f, 200.f, 45.f);
	m_light = new demorender::Light();
}

void PostProcessTesti2::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = m_pos * 0.4f * 250.f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * 30.f;
	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);
}

void PostProcessTesti2::drawScene(const std::string& textureName)
{
	glDepthMask(1);
	glEnable(GL_DEPTH_TEST);

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	drawBackground();

	Shader &s = g_shaders->getShader("pointlight");
	s.bind();
	m_pModel->bind(&s);

	g_textures->bindTexture(textureName, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::vec3 lightPos = m_camera->getT() + m_camera->getZ() * 30.f;

	m_light->setPosition(lightPos);
	s.setUniform3fv("light.position", 1, (float *)&m_light->getPosition());
	s.setUniform3fv("light.color", 1, (float *)&m_light->getColor());

	Math::srand(666);
	const int count = 3;
	glEnable(GL_DEPTH_TEST);
	for (int z = -count; z < count; z++)
	{
		for (int y = -count; y < count; y++)
		{
			for (int x = -count; x < count; x++)
			{
				if (x == 0 && y == 0 && z == 0)
					continue;

				glm::vec3 rot = glm::vec3(sinf(x + m_pos), cosf(y + m_pos * 0.15f), sinf(z + x + m_pos * 0.6f));
				glm::vec3 pos = glm::vec3(x, y, z) * 3.f + glm::vec3(Math::randFloat() - 0.5f, Math::randFloat() - 0.5f, Math::randFloat() - 0.5f) * 45.f;

				if (glm::distance(m_cameraPosition, pos) < 2.f)
					continue;

				glm::mat4 modelMatrix = glm::translate(pos) * glm::rotate(m_pos * 120, rot) * glm::scale(glm::vec3(5.f));
				s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
				s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(modelMatrix))));
				glDrawArrays(GL_TRIANGLES, 0, m_pModel->getMesh()->getVertexCount()); GL_DEBUG;
			}
		}
	}

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
	glm::mat4 skyboxTrans = glm::scale(glm::vec3(40.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(skyboxTrans))));
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
}

void PostProcessTesti2::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pSkybox->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pSkybox->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(90.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;

}
void PostProcessTesti2::draw()
{
	g_renderTargets->bindMain();
	 
	drawScene("chess");

	g_postProcess->addLens(0.15f, m_camera);
	//g_postProcess->addSSAO(m_camera->getNearPlane(), m_camera->getFarPlane());
	//g_postProcess->addDOF(0.5f + 0.4f * sinf(m_pos * 50.f), m_camera->getNearPlane(), m_camera->getFarPlane());
	//	g_renderDebug->drawTexture("postprocess_1024", 512, 0, 256.f);
	//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("postprocess_1024"), m_camera, 512 + 256, 256, 256.f);
//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
