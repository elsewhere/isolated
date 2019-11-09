#include "lightingtest.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void LightingTest::init()
{
	m_pModel  = new demorender::Model();
	m_pModel->setMesh("cube");

	m_camera = new demorender::Camera(1.f, 200.f, 45.f);
	m_light = new demorender::Light();
}

void LightingTest::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = m_pos * 25.f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * (35.f + 5 * sinf(m_pos * 10));
	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);
}

void LightingTest::draw()
{
	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	glActiveTexture(GL_TEXTURE0); GL_DEBUG;

	Shader &s = g_shaders->getShader("pointlight");
	s.bind();
	m_pModel->bind(&s);

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
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

				glm::mat4 modelMatrix = glm::translate(pos) * glm::rotate(m_pos * 80, rot);
				s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
				s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(modelMatrix))));
				glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
			}
		}
	}

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
	glm::mat4 skyboxTrans = glm::scale(glm::vec3(90.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(skyboxTrans))));
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;

	glBindVertexArray(0); GL_DEBUG;
}
