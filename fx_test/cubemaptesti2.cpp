#include "CubemapTesti2.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

namespace
{
	static const std::string skyboxTexture = "miramar";
}
void CubemapTesti2::init()
{
	m_pModel  = new demorender::Model();
	m_pModel->setMesh("cube");

	m_pModelSurrounding = new demorender::Model();
	m_pModelSurrounding->setMesh("asteroid");
	m_pModelSurrounding->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM | Mesh::UV_STREAM);

	m_pMesh = g_meshes->getMesh("cow-nonormals");
	m_pMesh->setStreamFlags(Mesh::VERTEX_STREAM | Mesh::NORMAL_STREAM);
	m_camera = new demorender::Camera(1.f, 200.f, 45.f);

//	m_pParticles = new demorender::CPUParticleSystem();
}

void CubemapTesti2::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a =  m_pos * 25.f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * (35.f + 5 * sinf(m_pos * 10));
	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);

//	m_pParticles->update();
}

void CubemapTesti2::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pModel->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pModel->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(90.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;

}
void CubemapTesti2::drawMesh()
{
	Shader &s = g_shaders->getShader("simplereflection");
	s.bind();
	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;

	m_pMesh->bind(&s);

	glm::vec3 rot = glm::vec3(m_pos * 11.f, m_pos * 16.f, -m_pos * 13.f);
	glm::mat4 model = glm::rotate(m_pos * 80, rot) * glm::scale(glm::vec3(4.1f));
	
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniform3fv("cameraPosition", 1, (float *)&m_camera->getT());
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	m_pMesh->draw();
}

void CubemapTesti2::drawSurroundings()
{

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;

	const float d = 20.f;

	positions.push_back(glm::vec3(-d, 0, 0));
	positions.push_back(glm::vec3( d, 0, 0));
	positions.push_back(glm::vec3( 0, -d, 0));
	positions.push_back(glm::vec3( 0, d, 0));
	positions.push_back(glm::vec3( 0, 0, -d));
	positions.push_back(glm::vec3( 0, 0, d));

	Math::srand(1);
	rotations.push_back(Math::randVectSphere());
	rotations.push_back(Math::randVectSphere());
	rotations.push_back(Math::randVectSphere());
	rotations.push_back(Math::randVectSphere());
	rotations.push_back(Math::randVectSphere());
	rotations.push_back(Math::randVectSphere());

	Shader &s = g_shaders->getShader("simpletexture");
	s.bind();

	g_textures->bindTexture("chess", GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	m_pModelSurrounding->bind(&s);

	for (int i = 0; i < 6; i++)
	{
		glm::mat4 modelMatrix = glm::translate(positions.at(i)) * glm::rotate(m_pos, rotations.at(i)) * glm::scale(glm::vec3(5.f));
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		m_pModelSurrounding->draw();
	}
}

void CubemapTesti2::drawScene()
{
	glDepthMask(1);
	glEnable(GL_DEPTH_TEST);

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	g_renderTargets->bindMain();

	for (int i = 0; i < 6; i++)
	{
		g_renderTargets->bindCubemapSide("dynamictest", i);
		drawBackground();
		drawSurroundings();
	}
	g_renderTargets->bindMain();
//	glEnable(GL_BLEND);
//	glDepthMask(0);
//	m_pParticles->draw(m_camera);
//	glDepthMask(1);
	drawSurroundings();
	drawMesh();

}

void CubemapTesti2::draw()
{
	drawScene();

//	g_postProcess->addInvert();
//	g_postProcess->addInvert();
	
//	float spread = 1.7f;// +0.5f * sinf(g_system->getTime() * 0.001f);
//	g_postProcess->addGlow(5, spread, spread, -0.5f, 0.8f);
}
