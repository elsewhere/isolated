#include "postprocesstesti.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void PostProcessTesti::init()
{
	m_pModel  = new demorender::Model();
	m_pModel->setMesh("asteroid");

	m_camera = new demorender::Camera(1.f, 200.f, 45.f);
	m_light = new demorender::Light();
}

void PostProcessTesti::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = m_pos * 25.f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * (35.f + 5 * sinf(m_pos * 10));
	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);
}

void PostProcessTesti::drawScene(const std::string& textureName)
{
	glDepthMask(1);
	glEnable(GL_DEPTH_TEST);

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	Shader &s = g_shaders->getShader("pointlight");
	s.bind();
	m_pModel->bind(&s);

	g_textures->bindTexture(textureName, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::vec3 lightPos = m_camera->getT() + m_camera->getZ() * sinf(m_pos * 40) * 30.f;

	m_light->setPosition(lightPos);
	s.setUniform3fv("light.position", 1, (float *)&m_light->getPosition());
	s.setUniform3fv("light.color", 1, (float *)&m_light->getColor());

	const int count = 5;
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
				glm::vec3 pos = glm::vec3(x, y, z) * 6.f;

				if (glm::distance(m_cameraPosition, pos) < 2.f)
					continue;

				glm::mat4 modelMatrix = glm::translate(pos) * glm::rotate(m_pos * 80, rot) * glm::scale(glm::vec3(5.f));
				s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
				s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(modelMatrix))));
				glDrawArrays(GL_TRIANGLES, 0, m_pModel->getMesh()->getVertexCount()); GL_DEBUG;
			}
		}
	}

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
	glm::mat4 skyboxTrans = glm::scale(glm::vec3(90.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(skyboxTrans))));
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
}

void PostProcessTesti::draw()
{
	g_renderTargets->bindMain();

	drawScene("chess");

//	g_postProcess->addInvert();
//	g_postProcess->addInvert();

//	g_postProcess->addInvert();
//	g_postProcess->addSobel(0.01f + 0.01f*sinf(m_pos * 150.f), 1.0f);

	float spread = 0.5f + 0.5f * sinf(g_system->getTime() * 0.001f);
	g_postProcess->addGlow(10, spread, spread, -0.5f, 2.0f);

	//	g_postProcess->addTest();
//	g_renderUtils->fullscreenQuad("testi.jpg");
//	drawScene("postprocess_256"); 

	//oikean ylänurkan koordinaatit apparently
/*
	g_renderDebug->drawTexture("downscale1", 0, 0, 256.f);
	g_renderDebug->drawTexture("downscale2", 256, 0, 256.f);
	g_renderDebug->drawTexture("downscale3", 512, 0, 256.f);
*/
	//	g_renderDebug->drawTexture("postprocess_1024", 512, 0, 256.f);
	//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("postprocess_1024"), m_camera, 512 + 256, 256, 256.f);

}
