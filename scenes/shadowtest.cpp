#include "ShadowTest.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/LineRenderer.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void ShadowTest::init()
{
	g_params->useNamespace("shadowtest");
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();
	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}
	
	m_shadowMap = std::make_unique<ShadowMap>();

	m_things.clear();

	for (int i = 0; i < 50; i++)
	{
		Thing* t = new Thing();

		MeshBuilder builder;
		builder.generateCube(1.f);

		float r = Math::randBetween(1.f, 50.f);
		float a = Math::randFloat() * 2 * 3.141592f;

		t->pos = glm::vec3(sinf(a) * r, Math::randBetween(2.5f, 5.5f), cosf(a) * r);

		t->pMesh = builder.getMesh(demorender::Mesh::Usage::STATIC);
		t->transform = glm::translate(t->pos);

		glm::vec3 col = glm::normalize(Math::randVectSphere());
		t->color = glm::normalize(glm::vec4(col, 1.f));
		
		t->scale = Math::randFloat(0.5f, 2.f);
		m_things.push_back(t);
	}

	if (m_terrain)
		delete m_terrain;

	const int zres = 100;
	const int xres = 100;
	const float scale = 100.f;

	MeshBuilder builder;
	builder.generatePlane(xres, zres, scale);
	m_terrain = builder.getMesh(Mesh::Usage::STATIC);

}

void ShadowTest::update()
{
	g_params->useNamespace("shadowtest");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	glm::vec3 cameraStart = glm::vec3(-30, 5.f, -40.f);
	glm::vec3 cameraEnd = glm::vec3(70, 10.f, 20.f);

	m_cameraPosition = cameraStart + (cameraEnd - cameraStart) * m_pos;

	m_cameraTarget = glm::vec3(0.f);
	m_cameraTarget.y = 0.f;// ::vec3(0.f);

	for (auto thing : m_things)
	{
		glm::vec3 pos = thing->pos;
		
		glm::vec3 angle = glm::normalize(glm::vec3(sinf(pos.x), cosf(pos.y), sinf(pos.z)) * 100.f);


		thing->transform = glm::translate(thing->pos) * glm::rotate(m_pos * 100.f, angle) * glm::scale(glm::vec3(thing->scale));
		
	}
}

void ShadowTest::drawGeometry(bool shadowPass)
{

	if (shadowPass)
	{
		Shader& s = g_shaders->getShader("depthonly");

		for (auto t : m_things)
		{
			s.bind();
			s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_shadowMap->getLightMatrix()); GL_DEBUG;
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			t->pMesh->bind(&s);
			t->pMesh->draw();
		}

	}
	else
	{
		Shader& s = g_shaders->getShader("thing");

		for (auto t : m_things)
		{
			s.bind();
			s.setUniform4fv("color", 1, (float *)&t->color);
			s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			s.setUniformMatrix4fv("lightMatrix", 1, GL_FALSE, (float *)&m_shadowMap->getLightMatrix()); GL_DEBUG;

//			glDepthMask(GL_FALSE);
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			t->pMesh->bind(&s);
			t->pMesh->draw();
		}
	}
}

void ShadowTest::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("plane");
	s.bind();

	g_textures->bindTexture("kivitesti", GL_TEXTURE0);
	g_textures->bindTexture(m_shadowMap->getDepthMapID(), GL_TEXTURE1);
	s.setUniform1i("texturemap", 0);
	s.setUniform1i("shadowMap", 1);

	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);
	s.setUniformMatrix4fv("lightMatrix", 1, false, (float *)&m_shadowMap->getLightMatrix());

	m_terrain->bind(&s);
	m_terrain->draw();

}
void ShadowTest::debug()
{
//	g_screenText.log<std::string>("lightPos", Math::toString(m_lightPos));
//	g_screenText.log<float>("m_lightValue", m_lightValue);
//	g_screenText.log<float>("m_lightRadius", m_lightRadius);
}

void ShadowTest::draw()
{
	g_params->useNamespace("shadowtest");

	m_light.setType(Light::Type::DIRECTIONAL);
	m_light.setPosition(glm::vec3(2.f, 10.f, 0.f));
	m_light.setTarget(glm::vec3(0.f));
	m_light.setUp(glm::vec3(1.f, 0.f, 0.f));

	m_shadowMap->prepare(m_light);
	drawGeometry(true);
	m_shadowMap->unbind();

	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	//draw geometry

	drawTerrain();
	drawGeometry(false);
//	m_lines->draw(m_camera, LineRenderer::Mode::LINES);

	m_shadowMap->debugDraw();

}
