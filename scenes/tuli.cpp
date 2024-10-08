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
	static const int numParticles = 1024 * 256;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KasvotParticles
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tuli::TuliParticles::TuliParticles() :
	GPUParticleSystem(numParticles)
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
	memset(m_pInitialData, 0, sizeof(float) * m_particleCount * m_particleSize);
}


void Tuli::init()
{
	m_numParticles = numParticles;

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

	const float rotation = Math::smoothStep(std::min<float>(m_pos * 3.f, 1.f), 0.f, 1.f);
	glm::mat4 cameraRotation = glm::rotate(sinf(m_pos * g_params->get<float>("camerarotationfreq")) * g_params->get<float>("camerarotationamount"), glm::vec3(0.f, 0.f, 1.f)) * rotation;

	m_cameraUp = Math::transform(m_cameraUp, cameraRotation);

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
	focusDistance += sinf(m_pos * g_params->get<float>("focusdistancespeed")) * g_params->get<float>("focusdistancerange");
	m_particles->addRenderShaderUniform("focusDistance", focusDistance);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);


	m_particles->update();
}

void Tuli::debug()
{
}

void Tuli::draw(RenderPass pass)
{
	g_params->useNamespace("Tuli");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);


		float fade = std::min(1.f, m_pos * 1.f);
		int particles = (int)(m_numParticles * fade);
		m_particles->draw(m_camera, particles);

//		g_postProcess->addSobel();

		float glow = std::min<float>(1.f, m_pos * 2.f);

		float pulse = sinf(g_sync->beat("pulsehalf").getValue()*3.141592f) * 0.2f * m_pos;
		float pulsequarter = sinf(g_sync->beat("pulsequarter").getValue()*3.141592f) * g_sync->beat("pulse").getValue() * m_pos;

		glow += pulse;

		int iterations = g_params->get<int>("glowiterations");
		float spread = g_params->get<float>("glowspread");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha") * glow;
		g_postProcess->addRadialGlow(iterations, spread, exponent, alpha);
		g_postProcess->addGlow(8, 0.001f, 0.001f, 0.3f * pulsequarter * m_pos, 1.f);
	}
	if (pass == RenderPass::AFTER_POST)
	{
		float fadevalue = g_sync->event("tulifadein").getValue() * (1.f - g_sync->event("tulifadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);

	}
}
