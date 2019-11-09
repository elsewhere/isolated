#include "Rivers.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOFCPUParticleSystem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Rivers::CPUParticleSystemDOF::CPUParticleSystemDOF():
	CPUParticleSystem::CPUParticleSystem(5000)
{
	setShader("particle_bokeh");
	setTexture("circle");
}

Rivers::CPUParticleSystemDOF::~CPUParticleSystemDOF()
{
}

void Rivers::CPUParticleSystemDOF::setInitialValues()
{
	for (int i = 0; i < m_particleCount; i++)
	{
		initParticle(i);
	}
}


void Rivers::CPUParticleSystemDOF::initParticle(int index)
{
	m_pParticleData[index].position = Math::randVectSphere() * 30.f * Math::randFloat();
	m_pParticleData[index].direction = Math::randVectSphere();
	m_pParticleData[index].size = Math::randBetween(0.1f, 0.1f);
	m_pParticleData[index].color = glm::vec4(1.f, 1.f, 1.f, Math::randBetween(0.1f, 0.5f));
	m_pParticleData[index].energy = m_pParticleData[index].maxEnergy = Math::randBetween(0.7f, 6.0f);
}

void Rivers::CPUParticleSystemDOF::update()
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

void Rivers::CPUParticleSystemDOF::draw(float focusDistance, demorender::Camera* pCamera)
{
	Shader& s = g_shaders->getShader(m_shader);
	s.bind();

	s.setUniform3fv("cameraPosition", 1, (float *)&pCamera->getT());
	s.setUniform1f("focusDistance", focusDistance);
	CPUParticleSystem::draw(pCamera);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rivers
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



namespace
{
}

void Rivers::init()
{
	m_camera = new demorender::Camera(1.f, 500.f, 60.f);
//	m_pParticles = new CPUParticleSystemDOF();
//	m_pParticles->setInitialValues();
}

void Rivers::update()
{
	g_params->useNamespace("river");
	m_cameraUp = g_params->getVector3("cameraup");
	m_cameraTarget = g_params->getVector3("cameratarget");
	m_cameraPosition = g_params->getVector3("cameraposition");

//	m_cameraPosition = glm::vec3(45, 0, 45);// glm::vec3(sinf(a), 0.f, cosf(a)) * 35.f;
//	m_cameraPosition.y = 5 * cosf(m_pos * 13.f);

//	m_pParticles->update();


}


void Rivers::draw()
{
	g_params->useNamespace("river");
	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	Shader &s = g_shaders->getShader("effect_river");
	s.bind();
	s.setUniform1f("time", m_pos);

	g_renderUtils->fullscreenQuad(s);
/*
//	m_pParticles->draw(30.f + 25.0f * sinf(m_pos * 125.f), m_camera);
//	g_postProcess->addInvert();
//	g_postProcess->addSobel();
//	g_postProcess->addLens(0.5f, m_camera);
//	float spread = 0.5f + 0.5f * sinf(g_system->getTime() * 0.001f);
//	g_postProcess->addGlow(10, spread, spread, -0.5f, 1.0f);
*/
}
