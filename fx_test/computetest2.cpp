#include "ComputeTest2.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;


void ComputeTest2::init()
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
	//	g_textures->getTexture("ComputeTest2");
	//	g_textures->bindTexture("computest")
	g_textures->bindTextureToImage("computetest", 0, GL_WRITE_ONLY);

	m_particleCount = 10000;

	Particle* temp = new Particle[m_particleCount];
	for (int i = 0; i < m_particleCount; i++)
	{
		temp[i].position = Math::randVectSphere() * Math::randFloat(20.f, 30.f);
		temp[i].direction = glm::normalize(temp[i].position);
		temp[i].energy = temp[i].maxEnergy = Math::randFloat(0.5f, 1.5f);
		temp[i].color = glm::vec4(Math::randFloat(0.f, 1.f), 
								  Math::randFloat(0.f, 1.f), 
								  Math::randFloat(0.f, 1.f), 
								  Math::randFloat(0.5f, 1.f));
	}

	m_buffer1 = new Buffer();
	m_buffer2 = new Buffer();

	m_buffer1->init(GL_SHADER_STORAGE_BUFFER, m_particleCount * sizeof(Particle), temp);
	m_buffer2->init(GL_SHADER_STORAGE_BUFFER, m_particleCount * sizeof(Particle), nullptr);

	delete[] temp;

}

void ComputeTest2::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = 0.f;// m_pos * 15.0f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * 10.f;
}


void ComputeTest2::draw()
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
