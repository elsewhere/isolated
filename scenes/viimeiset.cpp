#include "Viimeiset.h"
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


void Viimeiset::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	MeshBuilder builder;
	builder.generatePlane(xres, zres, scale);
	m_pMesh = builder.getMesh(Mesh::Usage::STATIC);

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");

}



void Viimeiset::update()
{
	g_params->useNamespace("Viimeiset");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camstart = g_params->get<glm::vec3>("camerapositionstart");
	glm::vec3 camend = g_params->get<glm::vec3>("camerapositionend");
	glm::vec3 targetstart = g_params->get<glm::vec3>("cameratargetstart");
	glm::vec3 targetend = g_params->get<glm::vec3>("cameratargetend");

	m_cameraPosition = Math::lerp<glm::vec3>(camstart, camend, m_pos);
	m_cameraTarget = Math::lerp<glm::vec3>(targetstart, targetend, m_pos);
}

void Viimeiset::debug()
{
}

void Viimeiset::drawGround()
{
	g_params->useNamespace("viimeiset");
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("effect_terrain");
	s.bind();

	g_textures->bindTexture("heightmap1", GL_TEXTURE0);
	g_textures->bindTexture("kivitesti", GL_TEXTURE1);
	s.setUniform1f("time", m_pos);
	s.setUniform1i("heightmap", 0);
	s.setUniform1i("stone", 1);
	s.setUniform1f("terrainScale", g_params->get<float>("terrainheightscale"));
	s.setUniform1f("terrainHeightScale", g_params->get<float>("terrainheightscale"));
	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	m_pMesh->bind(&s);
	m_pMesh->draw();
}

void Viimeiset::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pSkybox->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pSkybox->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniform1f("brightness", 1.0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(890.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
}

void Viimeiset::draw(RenderPass pass)
{
	g_params->useNamespace("Viimeiset");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

		drawBackground();
		drawGround();

		float focusDistance = g_params->get<float>("focusdistance");
		g_postProcess->addLens(focusDistance, m_camera);

/*
		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha");

		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha * m_pos);
*/
	}
	if (pass == RenderPass::AFTER_POST)
	{
		float fadevalue = g_sync->event("viimeisetfadein").getValue() * (1.f - g_sync->event("viimeisetfadeout").getValue());
		glEnable(GL_BLEND);

		glm::vec3 textpos = g_params->get<glm::vec3>("textpos");
		float textscale = Math::lerp<float>(g_params->get<float>("textscalestart"), g_params->get<float>("textscaleend"), m_pos);

		float credits = sinf(g_sync->event("credits").getValue() * 3.141592f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		if (credits> 0.001f)
			g_renderUtils->orthoImage("credits", glm::vec2(textpos.x, textpos.y), textscale, credits);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);
	}
}
