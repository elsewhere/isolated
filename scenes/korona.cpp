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

Korona::Sun::Sun() :
	CPUParticleSystem("particle_korona", "circle", 5),
	m_pos(0.f)
{

}

Korona::Sun::~Sun()
{

}

void Korona::Sun::setInitialValues()
{
	g_params->useNamespace("korona");
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticleData[i].position = Math::randVectSphere() * g_params->get<demomath::Range>("sunspread").getRandomValue() + g_params->get<glm::vec3>("sunposition");
		m_pParticleData[i].direction = Math::randVectSphere() * g_params->get<demomath::Range>("sunmovement").getRandomValue();
		m_pParticleData[i].size = g_params->get<demomath::Range>("sunsize").getRandomValue();
		m_pParticleData[i].color = glm::vec4(1.f, 1.f, 1.f, g_params->get<demomath::Range>("sunalpha").getRandomValue());
		m_pParticleData[i].energy = 1.f;
		m_pParticleData[i].maxEnergy = 1.f;
	}
}

void Korona::Sun::update()
{
	g_params->useNamespace("korona");

	const float speed = 0.01f;
	for (int i = 0; i < m_particleCount; i++)
	{

		glm::vec3 pos = Math::lerp(g_params->get<glm::vec3>("sunposition"), g_params->get<glm::vec3>("sunposition2"), m_pos);
		m_pParticleData[i].position = pos;
	}
	updateBuffers();
}


void Korona::init()
{
	g_params->useNamespace("korona");

	m_moonCamera = new demorender::Camera(1.f, 1000.f, 45.f);
	m_groundCamera = new demorender::Camera(1.f, 1000.f, 45.f);

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

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	builder.generatePlane(xres, zres, scale);
	m_pGround = builder.getMesh(Mesh::Usage::STATIC);

	m_sun = std::make_unique<Sun>();
	m_sun->setInitialValues();

	m_analyzer = std::make_unique <democore::Analyzer>(512, 16);


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
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_moonCamera->getCameraMatrix()); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model);

	m_pMoon->draw();
}

void Korona::drawGround()
{
	g_params->useNamespace("korona");
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
void Korona::drawLines()
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


void Korona::update()
{
	g_params->useNamespace("Korona");

	m_analyzer->update();

	const float rotation = Math::smoothStep(std::min<float>(m_pos * 3.f, 1.f), 0.f, 1.f);
	glm::mat4 cameraRotation = glm::rotate(sinf(m_pos * g_params->get<float>("camerarotationfreq")) * g_params->get<float>("camerarotationamount"), glm::vec3(0.f, 0.f, 1.f)) * rotation;

	glm::vec3 moonPositionStart = g_params->get<glm::vec3>("mooncamerapositionstart");
	glm::vec3 moonPositionEnd = g_params->get<glm::vec3>("mooncamerapositionend");
	glm::vec3 moonTargetStart = g_params->get<glm::vec3>("mooncameratargetstart");
	glm::vec3 moonTargetEnd = g_params->get<glm::vec3>("mooncameratargetend");

	m_moonCameraPosition = Math::lerp<glm::vec3>(moonPositionStart, moonPositionEnd, m_pos);
	m_moonCameraTarget = moonTargetStart + (moonTargetEnd - moonTargetStart) * m_pos;
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
	m_sun->setPos(m_pos);
	m_sun->update();



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

		m_moonCamera->lookAt(m_moonCameraPosition,
			m_moonCameraTarget,
			m_moonCameraUp);

		m_groundCamera->lookAt(m_groundCameraPosition,
			m_groundCameraTarget,
			m_groundCameraUp);

//		drawLines();
		m_particles->draw(m_moonCamera);
//		m_sun->draw(m_moonCamera);
		drawMoon();



		drawGround();

		g_postProcess->addSobel();

		const float glow = std::min<float>(1.f, m_pos * 2.f);
		int iterations = g_params->get<int>("glowiterations");
		float spreadx = g_params->get<float>("glowspreadx");
		float spready = g_params->get<float>("glowspready");
		float exponent = g_params->get<float>("glowexponent");
		float alpha = g_params->get<float>("glowalpha") + g_sync->event("koronareveal").getValue() * 4.5f;// *glow;

		float sum = m_analyzer->getSum(Analyzer::Mode::CURRENT);

//		g_debug << "sum = " << sum << "\n";

		g_postProcess->addEndOfTheWorld(m_pos * g_params->get<float>("endoftheworld"));
		g_postProcess->addGlow(iterations, spreadx, spready, exponent, alpha + sum * 1500.f);
	}
	if (pass == RenderPass::AFTER_POST)
	{
/*
		m_sun->draw(m_moonCamera);
		glDisable(GL_DEPTH_TEST);
		drawMoon();
		glEnable(GL_DEPTH_TEST);
*/
		float fadevalue = g_sync->event("koronafadein").getValue() * (1.f - g_sync->event("koronafadeout").getValue());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenFade(glm::vec4(0.f, 0.f, 0.f, 1.f - fadevalue));
		glDisable(GL_BLEND);

	}
}
