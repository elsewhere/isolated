#include "Tyhjyys.h"
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
// TyhjyysLight
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Tyhjyys::TyhjyysLight::TyhjyysLight()
{
	auto builder = std::make_unique<MeshBuilder>();

	builder->start(false);

	for (int i = 0; i < 60; i++)
	{
		glm::vec3 pos = Math::randVectSphere(60.f);
		glm::vec3 v1 = pos + Math::randVectSphere(1.4f);
		glm::vec3 v2 = pos + Math::randVectSphere(1.4f);
		glm::vec3 v3 = pos + Math::randVectSphere(1.4f);

		builder->addVertex(v1);
		builder->addVertex(v2);
		builder->addVertex(v3);
	}

	builder->end();
	m_pMesh = builder->getMesh(Mesh::Usage::STATIC);
}

Tyhjyys::TyhjyysLight::~TyhjyysLight()
{
}

void Tyhjyys::TyhjyysLight::update()
{

}

void Tyhjyys::TyhjyysLight::draw(Camera* camera)
{

	float lightScale = g_params->get<float>("tyhjyys", "lightscale");

	static float t = 0.f;

	t += 0.004f;
	glm::mat4 model = glm::rotate(t, glm::vec3(0.4f, 0.7f, 0.2f)) * glm::scale(glm::vec3(lightScale));

	Shader& s = g_shaders->getShader("singlecolor");
	s.bind();
	s.setUniform3f("color", 1.1f, 1.f, 1.f); 
	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float*)(&camera->getCameraMatrix()));
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	m_pMesh->bind(&s);
	m_pMesh->draw();

//	g_shaders->unbindShader();

}

void Tyhjyys::TyhjyysLight::debug()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tyhjyys
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	luo xz-planemeshi, y = 0
	triangle-meshi

	klassinen 

    0    1    2    3    4    5    6    7
	x----x----x----x----x----x----x----x
	|  / |  / |  / |  / |  / |  / |  / |
	| /  | /  | /  | /  | /  | /  | /  |
	x----x----x----x----x----x----x----x
	|  / |  / |  / |  / |  / |  / |  / |
	| /  | /  | /  | /  | /  | /  | /  |
	x----x----x----x----x----x----x----x
	|  / |  / |  / |  / |  / |  / |  / |
	| /  | /  | /  | /  | /  | /  | /  |
	x----x----x----x----x----x----x----x
	|  / |  / |  / |  / |  / |  / |  / |
	| /  | /  | /  | /  | /  | /  | /  |
	x----x----x----x----x----x----x----x

	1x1-kokoinen, transformin skaalalla skaalataan
	y-koordinaatti tulee heightmapista vertex shaderissa ?

*/

void Tyhjyys::createMesh()
{
	if (m_pMesh)
		delete m_pMesh;

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	MeshBuilder builder;
	builder.generatePlane(xres, zres, scale);
	m_pMesh = builder.getMesh(Mesh::Usage::STATIC);
}

void Tyhjyys::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	createMesh();

	m_light = std::make_unique<TyhjyysLight>();
	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();

	for (int i = 0; i < 300; i++)
	{
//		float t = (i / 300.f - 0.5f) * 2.f;
//		const float a = t * 15.f;
//		glm::vec3 v = glm::vec3(sinf(a) * 10.f, t * 100.f, cosf(a) * 10.f);
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");
}

void Tyhjyys::update()
{
	g_params->useNamespace("Tyhjyys");

	const float light_t = 0.5f + 0.5f * sinf(m_pos * 130.f);
	m_lightPos = g_params->get<glm::vec3>("lightpos");

	m_lightValue = 0.2f + (Math::randFloat(0, 1.f) * (light_t * 0.7f + 0.3f)) * 0.8f;
	m_lightRadius = 10.f + light_t * 100.f;
	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);


	m_light->setPos(m_lightPos);
	m_light->setIntensity(m_lightValue);

	const float radius = 100.f;
	const float height = 30.f;

	const float a = m_pos * 2.f;

	glm::vec3 cameraStart = glm::vec3(0, 0, -240.f);
	glm::vec3 cameraEnd = glm::vec3(0, 0, 240.f);

	m_cameraPosition = cameraStart + (cameraEnd - cameraStart) * m_pos;
	m_cameraPosition.y = height;
	m_cameraPosition += glm::vec3(sinf(a), 0.f, cosf(a)) * radius;

	m_cameraTarget = m_cameraPosition * 0.000f;
	m_cameraTarget.y = 0.f;// ::vec3(0.f);
}

void Tyhjyys::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("effect_ikuisuusplane");
	s.bind();

	g_textures->bindTexture("heightmap1", GL_TEXTURE0);
	g_textures->bindTexture("kivitesti", GL_TEXTURE1);
	s.setUniform1f("time", m_pos);
	s.setUniform1i("heightmap", 0);
	s.setUniform1i("stone", 1);
	s.setUniform1f("terrainScale", 1.2f);
	s.setUniform1f("terrainHeightScale", 24.f);
	s.setUniform3fv("lightpos", 1, (float *)&m_lightPos );

	s.setUniform1f("lightvalue", m_lightValue);
	s.setUniform1f("lightradius", m_lightRadius);

	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	m_pMesh->bind(&s);
	m_pMesh->draw();

//	m_lines->draw(m_camera, LineRenderer::Mode::LINE_STRIP);
}

void Tyhjyys::drawBackground()
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

void Tyhjyys::debug()
{


	m_light->debug();



	g_screenText.log<std::string>("lightPos", Math::toString(m_lightPos));
	g_screenText.log<float>("m_lightValue", m_lightValue);
	g_screenText.log<float>("m_lightRadius", m_lightRadius);
}

void Tyhjyys::draw()
{
	g_params->useNamespace("Tyhjyys");

	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	drawBackground();
//	drawTerrain();
	m_light->draw(m_camera);

	const float focus = 0.1f;

	g_postProcess->addRadialGlow( g_params->get<int>("glowiterations"), 
								  g_params->get<float>("glowspread"),
								  g_params->get<float>("glowexponent"),
								  g_params->get<float>("glowalpha"));

	//	g_postProcess->addRadial();
	//	g_postProcess->addLens(focus, m_camera);
	//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
