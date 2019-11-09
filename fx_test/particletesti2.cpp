#include "ParticleTesti2.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOFCPUParticleSystem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ParticleTesti2::CPUParticleSystemDOF::CPUParticleSystemDOF():
	CPUParticleSystem::CPUParticleSystem("particle_bokeh", "circle", 5000)
{
}

ParticleTesti2::CPUParticleSystemDOF::~CPUParticleSystemDOF()
{
}

void ParticleTesti2::CPUParticleSystemDOF::setInitialValues()
{
	for (int i = 0; i < m_particleCount; i++)
	{
		initParticle(i);
	}
}


void ParticleTesti2::CPUParticleSystemDOF::initParticle(int index)
{
	m_pParticleData[index].position = Math::randVectSphere() * 30.f * Math::randFloat();
	m_pParticleData[index].direction = Math::randVectSphere();
	m_pParticleData[index].size = Math::randBetween(0.1f, 0.1f);
	m_pParticleData[index].color = glm::vec4(1.f, 1.f, 1.f, Math::randBetween(0.1f, 0.5f));
	m_pParticleData[index].energy = m_pParticleData[index].maxEnergy = Math::randBetween(0.7f, 6.0f);
}

void ParticleTesti2::CPUParticleSystemDOF::update()
{
	const float speed = 0.01f;
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticleData[i].position += m_pParticleData[i].direction * speed;
		m_pParticleData[i].energy -= 0.01f;
		if (m_pParticleData[i].energy <= 0)
		{
			initParticle(i);
		}
	}
	updateBuffers();
}

void ParticleTesti2::CPUParticleSystemDOF::draw(float focusDistance, demorender::Camera* pCamera)
{
	Shader& s = g_shaders->getShader(m_shader);
	s.bind();

	s.setUniform3fv("cameraPosition", 1, (float *)&pCamera->getT());
	s.setUniform1f("focusDistance", focusDistance);
	CPUParticleSystem::draw(pCamera);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ParticleTesti2
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void ParticleTesti2::init()
{
	m_camera = new demorender::Camera(1.f, 100.f, 45.f);
	m_pParticles = new CPUParticleSystemDOF();
	m_pParticles->setInitialValues();
}

void ParticleTesti2::update()
{
	m_cameraTarget = glm::vec3(0, 0, 0);
	m_cameraUp = glm::vec3(0, 1, 0);

	const float a =  m_pos * 15.0f;
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * 35.f;
//	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);

	m_pParticles->update();
}


void ParticleTesti2::draw()
{
	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	m_pParticles->draw(30.f + 25.0f * sinf(m_pos * 125.f), m_camera);
//	g_postProcess->addInvert();
//	g_postProcess->addSobel();
//	g_postProcess->addLens(0.5f, m_camera);
	float spread = 0.5f + 0.5f * sinf(g_system->getTime() * 0.001f);
	g_postProcess->addGlow(10, spread, spread, -0.5f, 1.0f);
}
