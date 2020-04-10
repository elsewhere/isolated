#include "Kauneus.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

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

Kauneus::KauneusParticles::KauneusParticles() :
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

Kauneus::KauneusParticles::~KauneusParticles()
{

}

void Kauneus::KauneusParticles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	memset(m_pInitialData, 0, m_particleCount * m_particleSize * sizeof(float));
}


void Kauneus::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_particles = std::make_unique<KauneusParticles>();
	m_particles->setShaders("effect_kauneus", "effect_kauneusrender");
	m_particles->setInitialData();
	m_particles->createBuffers();

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");

	m_analyzer = std::make_unique<democore::Analyzer>(512, 16);

}



void Kauneus::update()
{
	g_params->useNamespace("Kauneus");

	m_analyzer->update();
	m_sum = m_analyzer->getSum(democore::Analyzer::Mode::WEIGHTED);

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	m_cameraPosition = g_params->get<glm::vec3>("cameraposition");// ::vec3(0.f, 0.f, -20.f);
	m_cameraTarget = g_params->get<glm::vec3>("cameratarget");

	glm::mat4 rot = glm::rotate(m_pos * g_params->get<float>("camrotation"), glm::normalize(glm::vec3(0.1f, 1.f, 0.f)));


	m_cameraPosition = Math::transform(m_cameraPosition, rot);
	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);


	float focusDistance = g_params->get<float>("focusdistance");
	focusDistance += focusDistance * sinf(m_pos * 20.f);
	m_particles->addRenderShaderUniform("focusDistance", focusDistance);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);


	m_particles->update();
}

void Kauneus::debug()
{
}

void Kauneus::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pSkybox->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pSkybox->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniform1f("brightness", 0.2f + m_pos * g_params->get<float>("backgroundbrightness")); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(590.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
}

void Kauneus::draw(RenderPass pass)
{
	g_params->useNamespace("Kauneus");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

		drawBackground();
		m_particles->draw(m_camera);

		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha");

		float power = (1.f - m_pos) * 2.5f + m_sum * 400 * (1.f - m_pos);
		g_postProcess->addEndOfTheWorld(power);
		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha * m_pos + m_sum * 1200 * m_pos);

//		g_postProcess->addSobel();
//		g_postProcess->addRadialGlow();
	}
	if (pass == RenderPass::AFTER_POST)
	{
		float f1 = g_sync->event("kauneusfadein").getValue();
		float f2 = 1.f - g_sync->event("kauneusfadeout").getValue();

		float fadevalue = f1 * f2;
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);
	}
}
