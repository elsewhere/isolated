#include "Maailma.h"
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

Maailma::MaailmaParticles::MaailmaParticles() :
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

Maailma::MaailmaParticles::~MaailmaParticles()
{

}

void Maailma::MaailmaParticles::setInitialData()
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


void Maailma::init()
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

	m_particles = std::make_unique<MaailmaParticles>();
	m_particles->setShaders("effect_maailma", "effect_maailmarender");
	m_particles->setInitialData();
	m_particles->createBuffers();
}



void Maailma::update()
{
	g_params->useNamespace("Maailma");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	const float rotation = Math::smoothStep(std::min<float>(m_pos * 3.f, 1.f), 0.f, 1.f);
//	glm::mat4 cameraRotation = glm::rotate(sinf(m_pos * g_params->get<float>("camerarotationfreq")) * g_params->get<float>("camerarotationamount"), glm::vec3(0.f, 0.f, 1.f)) * rotation;

//	m_cameraUp = Math::transform(m_cameraUp, cameraRotation);

	glm::vec3 camstart = g_params->get<glm::vec3>("camerapositionstart");
	glm::vec3 camend = g_params->get<glm::vec3>("camerapositionend");
	glm::vec3 targetstart = g_params->get<glm::vec3>("cameratargetstart");
	glm::vec3 targetend = g_params->get<glm::vec3>("cameratargetend");
	m_cameraPosition = Math::lerp<glm::vec3>(camstart, camend, m_pos);
	m_cameraTarget = Math::lerp<glm::vec3>(targetstart, targetend, m_pos);

	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	float effecttime = 0.3f + 0.7f * m_pos;
	m_particles->addLogicShaderUniform("effecttime", effecttime);

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

void Maailma::debug()
{
}

void Maailma::draw(RenderPass pass)
{
	g_params->useNamespace("Maailma");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);


		float fade = std::min(1.f, m_pos * 2.f);
		int particles = (int)(m_numParticles * fade);
		m_particles->draw(m_camera, particles);

//		g_postProcess->addSobel();

		const float glow = std::min<float>(1.f, m_pos * 2.f);
		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha") * glow;

		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha);
	}
	if (pass == RenderPass::POST)
	{
		float fadevalue = g_sync->event("maailmafadein").getValue() * (1.f - g_sync->event("maailmafadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);

	}
}
