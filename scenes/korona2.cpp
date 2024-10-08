#include "Korona2.h"
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

Korona2::Korona2Particles::Korona2Particles() :
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

Korona2::Korona2Particles::~Korona2Particles()
{

}

void Korona2::Korona2Particles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	float* dataptr = m_pInitialData;
	for (int i = 0; i < m_particleCount; i++)
	{
		glm::vec3 position = Math::randVectSphere() * 5.f;
//		glm::vec4 color = glm::vec4(1.f);
		float energy = Math::randFloat(0.5f, 1.5f);

		writeData<glm::vec3>(&dataptr, position);
//		writeData<glm::vec4>(&dataptr, color);
		writeData<float>(&dataptr, energy);
		writeData<float>(&dataptr, energy);
	}
}


void Korona2::init()
{
	g_params->useNamespace("Korona2");

	m_moonCamera = new demorender::Camera(1.f, 1000.f, 45.f);
	m_groundCamera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_particles = std::make_unique<Korona2Particles>();
	m_particles->setShaders("effect_korona", "effect_koronarender");
	m_particles->setInitialData();
	m_particles->createBuffers();

	m_feedback = std::make_unique<demofx::Feedback>("feedback");

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

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	builder.generatePlane(xres, zres, scale);
	m_pGround = builder.getMesh(Mesh::Usage::STATIC);

	m_analyzer = std::make_unique <democore::Analyzer>(512, 16);
}


void Korona2::drawMoon()
{
	Shader &s = g_shaders->getShader("singlecolor");
	s.bind();
	m_pMoon->bind(&s);

	float startsize = g_params->get<float>("moonsizestart");
	float endsize = g_params->get<float>("moonsizeend");

	float size = startsize + (endsize - startsize) * m_pos;

	glm::mat4 model = glm::scale(glm::vec3(size));

	s.setUniform4f("color", 0.f, 0.f, 0.f, 1.f);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_moonCamera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	m_pMoon->draw();
}

void Korona2::drawGround()
{
	g_params->useNamespace("Korona2");
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("effect_terrainkorona");
	s.bind();

	g_textures->bindTexture("heightmap1", GL_TEXTURE0);
	g_textures->bindTexture("kivitesti", GL_TEXTURE1);
	s.setUniform1f("time", m_pos);
	s.setUniform1i("heightmap", 0);
	s.setUniform1i("stone", 1);
	s.setUniform1f("terrainScale", g_params->get<float>("terrainscale"));
	s.setUniform1f("terrainHeightScale", g_params->get<float>("terrainheightscale"));
	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_groundCamera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	m_pGround->bind(&s);
	m_pGround->draw();
}
void Korona2::drawLines()
{

	Shader &s = g_shaders->getShader("singlecolor");
	s.bind();
	m_pLines->bind(&s);

	const float size = 1.f;
	glm::mat4 model = glm::scale(glm::vec3(size));

	s.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_moonCamera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	glDepthMask(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_pLines->draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(1);

}


void Korona2::update()
{
	g_params->useNamespace("Korona2");

	m_analyzer->update();

	m_sum = m_analyzer->getSum(Analyzer::Mode::WEIGHTED);

	const float rotation = Math::smoothStep(std::min<float>(m_pos * 3.f, 1.f), 0.f, 1.f);
	glm::mat4 cameraRotation = glm::rotate(sinf(m_pos * g_params->get<float>("camerarotationfreq")) * g_params->get<float>("camerarotationamount"), glm::vec3(0.f, 0.f, 1.f)) * rotation;

	glm::vec3 moonPositionStart = g_params->get<glm::vec3>("mooncamerapositionstart");
	glm::vec3 moonPositionEnd = g_params->get<glm::vec3>("mooncamerapositionend");
	glm::vec3 moonTargetStart = g_params->get<glm::vec3>("mooncameratargetstart");
	glm::vec3 moonTargetEnd = g_params->get<glm::vec3>("mooncameratargetend");

	m_moonCameraPosition = Math::lerp<glm::vec3>(moonPositionStart, moonPositionEnd, m_pos) + Math::randVectSphere() * 2.5f * m_sum * (1.f - m_pos); 
	m_moonCameraTarget = moonTargetStart + (moonTargetEnd - moonTargetStart) * m_pos + Math::randVectSphere() * 1.5f * m_sum * (1.f - m_pos);
	m_moonCameraUp = glm::vec3(0.f, 1.f, 0.f);

	glm::vec3 groundPositionStart = g_params->get<glm::vec3>("groundcamerapositionstart");
	glm::vec3 groundPositionEnd = g_params->get<glm::vec3>("groundcamerapositionend");
	glm::vec3 groundTargetStart = g_params->get<glm::vec3>("groundcameratargetstart");
	glm::vec3 groundTargetEnd = g_params->get<glm::vec3>("groundcameratargetend");

	m_groundCameraPosition = Math::lerp<glm::vec3>(groundPositionStart, groundPositionEnd, m_pos);
	m_groundCameraTarget = Math::lerp<glm::vec3>(groundTargetStart, groundTargetEnd, m_pos);
	m_groundCameraUp = glm::vec3(0.f, 1.f, 0.f);

	glm::mat4 modelMatrix = glm::mat4(1.f);

	m_particles->startFrame();

	//	m_particles->addLogicShaderUniform("tex", 0);

	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_moonCamera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_moonCamera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", modelMatrix);


	float focusstart = g_params->get<float>("focusdistancestart");
	float focusend = g_params->get<float>("focusdistanceend");


	float focus = Math::lerp<float>(focusstart, focusend, m_pos);

	m_particles->addRenderShaderUniform("focusDistance", focus);
	m_particles->addRenderShaderUniform("cameraPosition", m_moonCameraPosition);

	m_particles->update();

	m_feedback->update();
}

void Korona2::debug()
{
}

void Korona2::drawFeedback()
{
	m_moonCamera->lookAt(m_moonCameraPosition,
		m_moonCameraTarget,
		m_moonCameraUp);

	m_feedback->startFrame();
	m_particles->draw(m_moonCamera);
	m_feedback->endFrame();
}

void Korona2::draw(RenderPass pass)
{
	g_params->useNamespace("Korona2");

	if (pass == RenderPass::MAIN)
	{
//		drawFeedback();

		m_moonCamera->lookAt(m_moonCameraPosition,
			m_moonCameraTarget,
			m_moonCameraUp);

		m_groundCamera->lookAt(m_groundCameraPosition,
			m_groundCameraTarget,
			m_groundCameraUp);

//		drawLines();
		g_renderTargets->bindMain();
		m_particles->draw(m_moonCamera);
		drawMoon();

//		drawGround();

		g_postProcess->addSobel();

		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alphastart = g_params->get<float>("glowalpha");// *glow;
		float alphaend = g_params->get<float>("glowalphaend");


//		g_debug << "sum = " << sum << "\n";

		float alpha = Math::lerp(alphastart, alphaend, m_pos);

		int radialiterations = g_params->get<int>("radialiterations");
		float radialspread = g_params->get<float>("radialspread");
		float radialexponent = g_params->get<float>("radialexponent");
		float radialalpha = g_params->get<float>("radialalpha");// *glow;

		g_postProcess->addRadialGlow(radialiterations, radialspread, radialexponent, radialalpha + m_sum * 10.f);
		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha + (m_sum + m_sum * m_pos * 0.5f) * 100.f);
		g_postProcess->addEndOfTheWorld(1.0f + m_pos * 0.3f + m_sum * 20.f * (1.f - m_pos));
	}
	if (pass == RenderPass::AFTER_POST)
	{
//		m_feedback->draw();

		float fadevalue = g_sync->event("korona2fadein").getValue() * (1.f - g_sync->event("korona2fadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);
	}
}
