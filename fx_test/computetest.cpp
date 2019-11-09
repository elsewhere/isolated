#include "ComputeTest.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;


void ComputeTest::init()
{
	m_camera = new demorender::Camera(1.f, 100.f, 45.f);
	/*
		glGenTextures(1, &m_texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
	*/
	//	g_textures->getTexture("computetest");
	//	g_textures->bindTexture("computest")
	g_textures->bindTextureToImage("computetest", 0, GL_WRITE_ONLY);
}

void ComputeTest::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = 0.f;// m_pos * 15.0f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * 10.f;
}


void ComputeTest::draw()
{
	g_renderTargets->bindMain();

	Shader& compute = g_shaders->getShader("computetest");
	compute.bind();
	compute.setUniform1f("time", g_system->getTime() * 0.001f);
	glDispatchCompute(32, 32, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	g_renderDebug->drawTexture("computetest", 256, 256, 512.f);

//	m_pParticles->draw(m_camera);
//	g_postProcess->addLens(0.5f+0.5f*sinf(m_pos*140.f), m_camera);
//	float spread = 0.5f + 0.5f * sinf(g_system->getTime() * 0.001f);
//	g_postProcess->addGlow(10, spread, spread, -0.5f, 2.0f);
}
