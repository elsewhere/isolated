#include "Sauhu.h"
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

Sauhu::SauhuParticles::SauhuParticles() :
	GPUParticleSystem(1024 * 1024)
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

Sauhu::SauhuParticles::~SauhuParticles()
{

}

void Sauhu::SauhuParticles::setInitialData()
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


void Sauhu::init()
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

	m_particles = std::make_unique<SauhuParticles>();
	m_particles->setShaders("effect_sauhu", "effect_sauhurender");
	m_particles->setInitialData();
	m_particles->createBuffers();
}



void Sauhu::update()
{
	g_params->useNamespace("sauhu");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	m_cameraPosition = g_params->get<glm::vec3>("cameraposition");// ::vec3(0.f, 0.f, -20.f);
	m_cameraTarget = glm::vec3(0.f);

	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);


	float focusDistance = g_params->get<float>("focusdistance");
	focusDistance += focusDistance * sinf(m_pos * 150.f);
	m_particles->addRenderShaderUniform("focusDistance", focusDistance);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);


	m_particles->update();
}

void Sauhu::debug()
{
}

void Sauhu::draw(RenderPass pass)
{
	g_params->useNamespace("Sauhu");

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
	if (pass == RenderPass::POST)
	{
		float fadevalue = g_sync->event("startfadein").getValue() * (1.f - g_sync->event("startfadeout").getValue());
		glEnable(GL_BLEND);

		glm::vec3 grouppos = g_params->get<glm::vec3>("grouppos");
		glm::vec3 titlepos = g_params->get<glm::vec3>("titlepos");

		float grouptitle = sinf(g_sync->event("grouptitle").getValue() * 3.141592f);
		float demotitle = sinf(g_sync->event("demotitle").getValue() * 3.141592f);

		if (grouptitle > 0.001f)
			g_renderUtils->orthoImage("grouptitle", glm::vec2(grouppos.x, grouppos.y), 0.5f, grouptitle);
		if (demotitle > 0.001f)
			g_renderUtils->orthoImage("demotitle", glm::vec2(titlepos.x, titlepos.y), 0.5f, demotitle);



		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);
	}
}
