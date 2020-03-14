#include "Tuli.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/LineRenderer.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

namespace
{
	static const std::string skyboxTexture = "nebula";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KasvotParticles
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tuli::TuliParticles::TuliParticles() :
	GPUParticleSystem(1024 * 128)
{
	addLogicShaderAttribute({ "particlePosition", 3 });
	addLogicShaderAttribute({ "particleColor", 4 });
	addLogicShaderAttribute({ "particleEnergy", 1 });
	addLogicShaderAttribute({ "particleMaxEnergy", 1 });

	addRenderShaderAttribute({ "vertexPosition", 3 });
	addRenderShaderAttribute({ "vertexColor", 4 });
	addRenderShaderAttribute({ "vertexEnergy", 1 });
	addRenderShaderAttribute({ "vertexMaxEnergy", 1 });
}

Tuli::TuliParticles::~TuliParticles()
{

}

void Tuli::TuliParticles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	float* dataptr = m_pInitialData;
	for (int i = 0; i < m_particleCount; i++)
	{
		glm::vec3 position = Math::randVectSphere() * 5.f;
		glm::vec4 color = glm::vec4(1.f);
		float energy = Math::randFloat(0.5f, 1.5f);

		writeData<glm::vec3>(&dataptr, position);
		writeData<glm::vec4>(&dataptr, color);
		writeData<float>(&dataptr, energy);
		writeData<float>(&dataptr, energy);
	}
}


void Tuli::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();

	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}

	m_particles = std::make_unique<TuliParticles>();
	m_particles->setShaders("effect_tuli", "effect_tulirender");
	m_particles->setInitialData();
	m_particles->createBuffers();
}



void Tuli::update()
{
	g_params->useNamespace("Tuli");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	m_cameraPosition = g_params->get<glm::vec3>("cameraposition");// ::vec3(0.f, 0.f, -20.f);
	m_cameraTarget = g_params->get<glm::vec3>("cameratarget");

	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);


	float focusDistance = g_params->get<float>("focusdistance");
	focusDistance += focusDistance * sinf(m_pos * g_params->get<float>("focusdistancespeed")) * g_params->get<float>("focusdistancerange");
	m_particles->addRenderShaderUniform("focusDistance", focusDistance);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);


	m_particles->update();
}

void Tuli::debug()
{
}

void Tuli::draw(RenderPass pass)
{
	g_params->useNamespace("Kasvot");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

		m_particles->draw(m_camera);


//		g_postProcess->addSobel();
//		g_postProcess->addRadialGlow(5, 0.001f);

	}
}
