#include "ParticleTesti3.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParticleTesti3
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void ParticleTesti3::init()
{
	m_camera = new demorender::Camera(1.f, 100.f, 45.f);
	m_pParticles = new demofx::GPUParticleSystem(200000);
	m_pParticles->setShader("gpuparticletest");
	m_pParticles->setInitialData();
	m_pParticles->createBuffers();
}

void ParticleTesti3::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a = 0.f;// m_pos * 15.0f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * 10.f;

	m_pParticles->update();
}


void ParticleTesti3::draw()
{
	g_renderTargets->bindMain();
	 
	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	m_pParticles->draw(m_camera);
//	g_postProcess->addLens(0.5f+0.5f*sinf(m_pos*140.f), m_camera);
//	float spread = 0.5f + 0.5f * sinf(g_system->getTime() * 0.001f);
//	g_postProcess->addGlow(10, spread, spread, -0.5f, 2.0f);
}
