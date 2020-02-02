#include "ShadowTest2.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/LineRenderer.h"
#include "../render/ShadowMap.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void ShadowTest2::init()
{
	g_params->useNamespace("ShadowTest2");
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();
	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}
	
	ShadowMapParameters params;

	params.width = 1024;
	params.height = 1024; 
	params.cubeMap = true;

	m_shadowMap = std::make_unique<ShadowMap>(params);

	m_things.clear();

	MeshBuilder builder;
	builder.generateCube(1.f);


	m_thingMesh = builder.getMesh(demorender::Mesh::Usage::STATIC);
	for (int i = 0; i < 50; i++)
	{
		Thing* t = new Thing();

		float r = Math::randBetween(1.f, 50.f);

		t->pos = Math::randVectSphere(r);

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

//	MeshBuilder builder;
	
	builder.generateCube(100.f);
	m_terrain = builder.getMesh(Mesh::Usage::STATIC);
}

void ShadowTest2::update()
{
	g_params->useNamespace("ShadowTest2");

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
	updateLights();
}

void ShadowTest2::updateLights()
{
	const float a = m_pos * 20.f;
	const float radius = 15.f;
	const float height = 15.f;
	m_pointLight.setType(Light::Type::POINT);
	m_pointLight.setPosition(glm::vec3(radius * sinf(a), height, radius * cosf(a)));
	m_pointLight.setTarget(glm::vec3(0.f));
	m_pointLight.setUp(glm::vec3(1.f, 0.f, 0.f));

}

void ShadowTest2::drawGeometry(bool shadowPass)
{
	if (shadowPass)
	{
		Shader& s = g_shaders->getShader("depthonly");

		s.bind();
		m_thingMesh->bind(&s);
		for (auto t : m_things)
		{
			s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_shadowMap->getLightMatrix()); GL_DEBUG;
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			m_thingMesh->draw();
		}
	}
	else
	{
		Shader& s = g_shaders->getShader("thing");

		s.bind();
		m_thingMesh->bind(&s);
		for (auto t : m_things)
		{
			s.setUniform4fv("color", 1, (float *)&t->color);
			s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			s.setUniformMatrix4fv("lightMatrix", 1, GL_FALSE, (float *)&m_shadowMap->getLightMatrix()); GL_DEBUG;

			m_thingMesh->draw();
		}

		//draw point light		
		glm::mat4 model = glm::translate(m_pointLight.getPosition()) * glm::scale(glm::vec3(1.f));;

		s.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
		s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("lightMatrix", 1, GL_FALSE, (float *)&m_shadowMap->getLightMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model); GL_DEBUG;

		m_thingMesh->bind(&s);
		m_thingMesh->draw();
	}
}

void ShadowTest2::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("shadowcube");
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
void ShadowTest2::debug()
{
	m_shadowMap->debugDraw();
}

void ShadowTest2::draw()
{
	g_params->useNamespace("ShadowTest2");

	m_shadowMap->prepare(m_pointLight);
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

}
