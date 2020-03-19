#include "Korona.h"
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

Korona::KoronaParticles::KoronaParticles() :
	GPUParticleSystem(1024 * 256)
{
	addLogicShaderAttribute({ "particlePosition", 3 });
//	addLogicShaderAttribute({ "particleColor", 4 });
	addLogicShaderAttribute({ "particleEnergy", 1 });
	addLogicShaderAttribute({ "particleMaxEnergy", 1 });

	addRenderShaderAttribute({ "vertexPosition", 3 });
//	addRenderShaderAttribute({ "vertexColor", 4 });
	addRenderShaderAttribute({ "vertexEnergy", 1 });
	addRenderShaderAttribute({ "vertexMaxEnergy", 1 });
}

Korona::KoronaParticles::~KoronaParticles()
{

}

void Korona::KoronaParticles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	float* dataptr = m_pInitialData;
	for (int i = 0; i < m_particleCount; i++)
	{
		glm::vec3 position = glm::vec3(0.f);// ::randVectSphere() * 5.f;
//		glm::vec4 color = glm::vec4(1.f);
		float energy = Math::randFloat(0.5f, 1.5f);

		writeData<glm::vec3>(&dataptr, position);
//		writeData<glm::vec4>(&dataptr, color);
		writeData<float>(&dataptr, energy);
		writeData<float>(&dataptr, energy);
	}
}


void Korona::init()
{
	g_params->useNamespace("korona");

	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();

	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}

	m_particles = std::make_unique<KoronaParticles>();
	m_particles->setShaders("effect_korona", "effect_koronarender");
	m_particles->setInitialData();
	m_particles->createBuffers();

	m_pMoon = g_meshes->getMesh("sphere");
	m_pMoon->setStreamFlags(Mesh::VERTEX_STREAM);

	MeshBuilder builder;
	int linecount = g_params->get<int>("linecount");
	float linelength = g_params->get<float>("linelength");
	Range linewidth = g_params->get<Range>("linewidth");

	float linedist = g_params->get<float>("linedistance");
	builder.start(false);
	for (int i = 0; i < linecount; i++)
	{
		float t = (float)i / (float)linecount;
		float a = t * 3.141592f * 2;

		float a1 = a - linewidth.getRandomValue();
		float a2 = a + linewidth.getRandomValue();

		glm::vec3 v1 = glm::vec3(0.f);
		glm::vec3 v2 = glm::vec3(sinf(a1), cosf(a1), 0.f) * linelength;
		glm::vec3 v3 = glm::vec3(sinf(a2), cosf(a2), 0.f) * linelength;

		v1.z = linedist;
		v2.z = linedist;
		v3.z = linedist;
		builder.addTriangleVertex(v1, v2, v3);
	}
	m_pLines = builder.getMesh(Mesh::Usage::STATIC);
}


void Korona::drawMoon()
{
	Shader &s = g_shaders->getShader("singlecolor");
	s.bind();
	m_pMoon->bind(&s);

	float startsize = g_params->get<float>("moonsizestart");
	float endsize = g_params->get<float>("moonsizeend");

	float size = startsize + (endsize - startsize) * m_pos;

	glm::mat4 model = glm::scale(glm::vec3(size));

	s.setUniform4f("color", 0.f, 0.f, 0.f, 1.f);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	m_pMoon->draw();
}

void Korona::drawLines()
{

	Shader &s = g_shaders->getShader("singlecolor");
	s.bind();
	m_pLines->bind(&s);

	const float size = 1.f;
	glm::mat4 model = glm::scale(glm::vec3(size));

	s.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	glDepthMask(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_pLines->draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(1);

}


void Korona::update()
{
	g_params->useNamespace("Korona");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	const float rotation = Math::smoothStep(std::min<float>(m_pos * 3.f, 1.f), 0.f, 1.f);
	glm::mat4 cameraRotation = glm::rotate(sinf(m_pos * g_params->get<float>("camerarotationfreq")) * g_params->get<float>("camerarotationamount"), glm::vec3(0.f, 0.f, 1.f)) * rotation;

	m_cameraUp = Math::transform(m_cameraUp, cameraRotation);

	m_cameraPosition = g_params->get<glm::vec3>("cameraposition");// ::vec3(0.f, 0.f, -20.f);
	glm::vec3 targetStart = g_params->get<glm::vec3>("cameratargetstart");
	glm::vec3 targetEnd = g_params->get<glm::vec3>("cameratargetend");

	m_cameraTarget = targetStart + (targetEnd - targetStart) * m_pos;

	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);


	float focusstart = g_params->get<float>("focusdistancestart");
	float focusend = g_params->get<float>("focusdistanceend");


	float focus = Math::lerp<float>(focusstart, focusend, m_pos);

	m_particles->addRenderShaderUniform("focusDistance", focus);
	m_particles->addRenderShaderUniform("cameraPosition", m_cameraPosition);


	m_particles->update();
}

void Korona::debug()
{
}

void Korona::draw(RenderPass pass)
{
	g_params->useNamespace("Korona");

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

//		drawLines();
		m_particles->draw(m_camera);
		drawMoon();

//		g_postProcess->addSobel();

		const float glow = std::min<float>(1.f, m_pos * 2.f);
		int iterations = g_params->get<int>("glowiterations");
		float spread = g_params->get<float>("glowspread");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha");// *glow;

		g_postProcess->addSobel();
		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

//		drawLines();
		g_postProcess->addGlow(iterations, spread, spread, exponent, alpha);
	}
	if (pass == RenderPass::POST)
	{
		float fadevalue = g_sync->event("koronafadein").getValue() * (1.f - g_sync->event("koronafadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);

	}
}
