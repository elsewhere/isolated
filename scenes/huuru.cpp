#include "Huuru.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void Huuru::init()
{
	m_camera = new demorender::Camera(1.f, 200.f, 45.f);
	m_light = new demorender::Light();

	Mesh *test = new Mesh();

	test->generateCube();
	g_meshes->addMesh("test", test);

	m_pModel = new demorender::Model();
	m_pModel->setMesh("test");

	m_trees.clear();

	for (int i = 0; i < 15; i++)
	{
		float t = i / 15.f;
		float a = t * 2 * 3.141592f;

		glm::vec3 pos = glm::vec3(sinf(a), 0.f, cosf(a)) * 15.f;
		demorender::Mesh *pMesh = generateTreeMesh(i);
		demorender::Model *pTree = new demorender::Model();
		pTree->setTransform(glm::translate(pos));
		pTree->setMesh(pMesh);
		m_trees.push_back(pTree);
	}
}

demorender::Mesh *Huuru::generateTreeMesh(int index)
{
	Mesh *pMesh = new Mesh();
	pMesh->generateCube();
	return pMesh;
}

void Huuru::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	m_cameraPosition = glm::vec3(0, 7.f, -30.f);
}

void Huuru::draw()
{
	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	glm::vec3 lightPos = glm::vec3(0, 0, 0);// m_camera->getT() + m_camera->getZ() * sinf(m_pos * 40) * 30.f;
	m_light->setPosition(lightPos);

	drawBackground();
	drawFire();

}

void Huuru::drawFire()
{

}
void Huuru::drawBackground()
{
	glActiveTexture(GL_TEXTURE0); GL_DEBUG;
	glEnable(GL_DEPTH_TEST);

	Shader &s = g_shaders->getShader("pointlight");
	s.bind();

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	s.setUniform3fv("light.position", 1, (float *)&m_light->getPosition());
	s.setUniform3fv("light.color", 1, (float *)&m_light->getColor());

	for (auto t : m_trees)
	{
		t->bind(&s);
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->getTransform()); GL_DEBUG;
		s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(t->getTransform()))));
		t->draw(Mesh::TRIANGLES);
	}
	/*
	m_pModel->bind(&s);

	for (int z = -5; z < 5; z++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				if (x == 0 && y == 0 && z == 0)
					continue;

				glm::vec3 rot = glm::vec3(sinf(x + m_pos), cosf(y + m_pos * 0.15f), sinf(z + x + m_pos * 0.6f));
				glm::vec3 pos = glm::vec3(x, y, z) * 3.f;

				if (glm::distance(m_cameraPosition, pos) < 2.f)
					continue;

				glm::mat4 modelMatrix = glm::translate(pos) * glm::rotate(m_pos * 80, rot) * glm::scale(glm::vec3(0.4f));
				s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
				s.setUniformMatrix3fv("normalMatrix", 1, GL_FALSE, (float *)&glm::transpose(glm::inverse(mat3(modelMatrix))));
//				s.setUniform4fv("color", 1, (float *)&glm::vec4(1.f));
				m_pModel->draw();
			}
		}
	}
	*/

	glBindVertexArray(0); GL_DEBUG;
}
