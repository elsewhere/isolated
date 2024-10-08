#include "Kasvot.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/LineRenderer.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

namespace
{
	static const std::string skyboxTexture = "fjaraenv";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KasvotParticles
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Kasvot::KasvotParticles::KasvotParticles(int count) :
	GPUParticleSystem(count)
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

Kasvot::KasvotParticles::~KasvotParticles()
{

}

void Kasvot::KasvotParticles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	memset(m_pInitialData, 0, m_particleCount * m_particleSize * sizeof(float));
	/*
	float* dataptr = m_pInitialData;
	for (int i = 0; i < m_particleCount; i++)
	{
		glm::vec3 position = glm::vec3(0.f);// Math::randVectSphere() * 5.f;
		glm::vec4 color = glm::vec4(1.f);
		float energy = Math::randFloat(0.5f, 1.5f);

		writeData<glm::vec3>(&dataptr, position);
		writeData<glm::vec4>(&dataptr, color);
		writeData<float>(&dataptr, energy);
		writeData<float>(&dataptr, energy);
	}
	*/
}

void Kasvot::createMesh()
{
	if (m_pMesh)
		delete m_pMesh;

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	MeshBuilder *builder = new MeshBuilder();
	builder->generatePlane(xres, zres, scale);
	m_pMesh = builder->getMesh(Mesh::Usage::STATIC);
}

void Kasvot::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	createMesh();

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");

	m_particles = std::make_unique<KasvotParticles>(1024 * 512);
	m_particles->setShaders("effect_kasvotparticle", "effect_kasvotparticlerender");
	m_particles->setInitialData();
	m_particles->createBuffers();

	m_particles2 = std::make_unique<KasvotParticles>(1024 * 256);
	m_particles2->setShaders("effect_kasvotparticle2", "effect_kasvotparticlerender2");
	m_particles2->setInitialData();
	m_particles2->createBuffers();

	m_particles3 = std::make_unique<KasvotParticles>(1024 * 256);
	m_particles3->setShaders("effect_kasvotparticle3", "effect_kasvotparticlerender3");
	m_particles3->setInitialData();
	m_particles3->createBuffers();


	m_analyzer = std::make_unique <democore::Analyzer>(512, 16);
}

void Kasvot::update()
{
	g_params->useNamespace("Kasvot");

	m_analyzer->update();
	m_sum = m_analyzer->getSum(Analyzer::Mode::WEIGHTED);

	glm::vec3 camstart = g_params->get<glm::vec3>("camerapositionstart");
	glm::vec3 camend = g_params->get<glm::vec3>("camerapositionend");
	glm::vec3 targetstart = g_params->get<glm::vec3>("cameratargetstart");
	glm::vec3 targetend = g_params->get<glm::vec3>("cameratargetend");

	float shake = g_sync->event("kasvotshake").getValue();
	m_cameraPosition = Math::lerp<glm::vec3>(camstart, camend, m_pos) + Math::randVectSphere() * 2.0f * m_sum * shake;
	m_cameraTarget = Math::lerp<glm::vec3>(targetstart, targetend, m_pos) + Math::randVectSphere() * 1.3f * m_sum * shake;
	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	glm::mat4 modelMatrix = glm::mat4(1.f);// glm::rotate(m_pos * -70.f, glm::vec3(0.3f, 1.0f, 0.2f)) * glm::scale(vec3(1.f));
	glm::mat4 modelMatrix2 = glm::translate(g_params->get<glm::vec3>("particle2position"));// glm::rotate(m_pos * -70.f, glm::vec3(0.3f, 1.0f, 0.2f)) * glm::scale(vec3(1.f));
	glm::mat4 modelMatrix3 = glm::translate(g_params->get<glm::vec3>("particle3position"));// glm::rotate(m_pos * -70.f, glm::vec3(0.3f, 1.0f, 0.2f)) * glm::scale(vec3(1.f));

	m_particles->startFrame();
	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);

	m_particles->addLogicShaderUniform("tex", 0);

	float fade = g_sync->event("kasvotchange").getValue() * (0.9f + 0.1f * (0.5f + 0.5f * sinf(g_sync->beat("pulseeighth").getValue() * 3.141592f)));

	m_particles2->startFrame();
	m_particles2->addRenderShaderUniform("tex", 0);
	m_particles2->addRenderShaderUniform("fade", fade);
	m_particles2->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles2->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles2->addRenderShaderUniform("modelMatrix", modelMatrix2);

	m_particles2->addLogicShaderUniform("tex", 0);

	m_particles3->startFrame();
	m_particles3->addRenderShaderUniform("tex", 0);
	m_particles3->addRenderShaderUniform("fade", fade);
	m_particles3->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles3->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles3->addRenderShaderUniform("modelMatrix", modelMatrix3);

	m_particles3->addLogicShaderUniform("tex", 0);

	float t = g_sync->event("kasvotfadechange").getValue();

	float a = Math::lerp<float>(g_sync->beat("pulsequarter").getValue(), g_sync->beat("pulseeighth").getValue(), t);// *(1.f - g_sync->event("kasvotchange").getValue());
	float focusDistance = g_params->get<float>("focusdistance");
//	focusDistance += sinf(m_pos * g_params->get<float>("focusdistancespeed")) * g_params->get<float>("focusdistancerange");
	focusDistance += sinf(a * 3.141592f) * g_params->get<float>("focusdistancerange");
	m_particles->addRenderShaderUniform("focusDistance", focusDistance);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);

	m_particles->update();
	m_particles2->update();
	m_particles3->update();
}

void Kasvot::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pSkybox->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pSkybox->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(290.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;

}

void Kasvot::debug()
{
}

void Kasvot::draw(RenderPass pass)
{
	if (pass == RenderPass::MAIN)
	{
		g_params->useNamespace("Kasvot");

		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

//		drawBackground();
		m_particles->draw(m_camera);
		if (g_sync->event("kasvotchange").hasPassed())
		{
			m_particles2->draw(m_camera);
			m_particles3->draw(m_camera);

		}

		float change = g_sync->event("kasvotchange").getValue();
		const float glow = std::min<float>(1.f, m_pos * 2.f);
		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha") * glow + change * 1.5f;

		g_postProcess->addEndOfTheWorld(change * 1.2f);
		g_postProcess->addRadialGlow(20, 0.002f , 1.f, 0.23f * change * (m_sum * 160.f));
		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha);

		//const float focus = 0.1f;

		//	g_postProcess->addRadialGlow( g_params->get<int>("glowiterations"), 
		//								  g_params->get<float>("glowspread"),
		//								  g_params->get<float>("glowexponent"),
		//								  g_params->get<float>("glowalpha"));

		//g_postProcess->addSobel();
			//	g_postProcess->addRadial();
			//	g_postProcess->addLens(focus, m_camera);
			//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

	}
	if (pass == RenderPass::AFTER_POST)
	{

		float fadevalue = g_sync->event("kasvotfadein").getValue() * (1.f - g_sync->event("kasvotfadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);

	}


}
