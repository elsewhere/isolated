#include "DynamicCubeTest.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/CubemapRenderer.h"
#include "../render/LineRenderer.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

void DynamicCubeTest::init()
{
	g_params->useNamespace("DynamicCubeTest");
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();
	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}
	
	m_things.clear();

	MeshBuilder builder;
	builder.generateCube(1.f);

	m_thingMesh = builder.getMesh(demorender::Mesh::Usage::STATIC);
	m_thingMesh->setStreamFlags(Mesh::VERTEX_STREAM);

	for (int i = 0; i < 50; i++)
	{
		Thing* t = new Thing();

		float r = Math::randBetween(10.f, 50.f);

		t->pos = Math::randVectSphere(r);

		t->transform = glm::translate(t->pos);

		glm::vec3 col = glm::normalize(Math::randVectSphere());
		t->color = glm::normalize(glm::vec4(col, 1.f));
		
		t->scale = Math::randFloat(0.5f, 2.f) * 2.f;
		m_things.push_back(t);
	}

	if (m_terrain)
		delete m_terrain;

	const int zres = 100;
	const int xres = 100;
	const float scale = 100.f;

//	MeshBuilder builder;
	
	builder.generateCube(60.f);
	m_terrain = builder.getMesh(Mesh::Usage::STATIC);

	m_pointLight.setType(Light::Type::POINT);

	builder.start(false);
	builder.generateCube(1.f);

	m_reflector = builder.getMesh(Mesh::Usage::STATIC);

	m_cubemapRenderer = std::make_unique<CubemapRenderer>();

}

void DynamicCubeTest::update()
{
	g_params->useNamespace("DynamicCubeTest");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

//	glm::vec3 cameraStart = glm::vec3(-30, 5.f, -20.f);
//	glm::vec3 cameraEnd = glm::vec3(40, 10.f, 20.f);


	float a = -m_pos * 30.f;

	glm::vec3 c = glm::vec3(sinf(a), 0.f, cosf(a)) * 15.f;

	c.y = 8.f;

	m_cameraPosition = c;
//	glm::vec3 cameraStart = glm::vec3(5.f, 7.f, -7.f);
//	glm::vec3 cameraEnd = cameraStart;
//	m_cameraPosition = cameraStart + (cameraEnd - cameraStart) * m_pos;

	m_cameraTarget = glm::vec3(0.f);
	m_cameraTarget.y = 0.f;// ::vec3(0.f);

	for (auto thing : m_things)
	{
		glm::vec3 pos = thing->pos;
		
		glm::vec3 globalAngle = glm::normalize(glm::vec3(sinf(m_pos * 43.f), 1.f, cosf(m_pos * 37.f)));
		glm::vec3 angle = glm::normalize(glm::vec3(sinf(pos.x), cosf(pos.y), sinf(pos.z)) * 100.f);
		thing->transform = glm::rotate(m_pos * 100.f, glm::vec3(0.f, 1.f, 0.f)) * 
						   glm::translate(thing->pos) * 
						   glm::rotate(m_pos * 160.f, angle) * 
						   glm::scale(glm::vec3(thing->scale));
	}
	updateLights();
}

void DynamicCubeTest::updateLights()
{
	const float a = m_pos * 20.f;
	const float radius = 15.f;
	const float height = 0.f;
	m_pointLight.setType(Light::Type::POINT);
	m_pointLight.setPosition(glm::vec3(0.f));
//	m_pointLight.setPosition(glm::vec3(radius * sinf(a), height, radius * cosf(a)));
	m_pointLight.setTarget(glm::vec3(0.f));
	m_pointLight.setUp(glm::vec3(1.f, 0.f, 0.f));

}

void DynamicCubeTest::drawGeometry(bool shadowPass, glm::mat4 cameraMatrix)
{
	Shader& s = g_shaders->getShader("simplecubething");

	s.bind();
	m_thingMesh->bind(&s);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&cameraMatrix); GL_DEBUG;
		
	for (auto t : m_things)
	{
		s.setUniform4fv("color", 1, (float *)&t->color);
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
		m_thingMesh->draw();
	}

}

void DynamicCubeTest::drawTerrain()
{
	glm::mat4 model = glm::scale(glm::vec3(8.f));// ::mat4(1.f);

	Shader& s = g_shaders->getShader("simpletexture");
	s.bind();

	g_textures->bindTexture("kivitesti", GL_TEXTURE0);
	s.setUniform1i("tex", 0);

	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	/*
		Shader& s = g_shaders->getShader("onlycubemap");
		s.bind();
		g_textures->bindCubemap("dynamictest", GL_TEXTURE0);
		s.setUniform1i("cubeMap", 0);
		s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
		s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);
	*/
	m_terrain->bind(&s);
	m_terrain->draw();

}
void DynamicCubeTest::debug()
{
}

void DynamicCubeTest::drawReflector()
{
	Shader& reflectionShader = g_shaders->getShader("cubereflection");
	reflectionShader.bind();
	m_thingMesh->bind(&reflectionShader);

	glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix *= glm::scale(glm::vec3(3.f));
	reflectionShader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
	reflectionShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
	reflectionShader.setUniform3fv("cameraPosition", 1, (float *)&glm::vec3(0.f)); GL_DEBUG;
	reflectionShader.setUniform1i("textureMap", 0);
	reflectionShader.setUniform1i("cubeMap", 1);

	g_textures->bindTexture("kivitesti", GL_TEXTURE0);
	g_textures->bindCubemap("dynamictest", GL_TEXTURE1);

	m_reflector->draw();

}

void DynamicCubeTest::draw(Scene::RenderPass pass)
{
	g_params->useNamespace("DynamicCubeTest");

	if (pass == RenderPass::REFLECTION)
	{
		//render reflection 
		m_cubemapRenderer->setPosition(m_pointLight.getPosition(), m_camera->getNearPlane(), m_camera->getFarPlane());

		for (int i = 0; i < 6; i++)
		{
			Cubemap::Side side = static_cast<Cubemap::Side>(i);
			m_cubemapRenderer->bind("dynamictest", side);
			drawTerrain();
			drawGeometry(false, m_cubemapRenderer->getTransform(side));
		}
	}

	if (pass == RenderPass::MAIN)
	{
		g_renderTargets->bindMain();

		m_camera->lookAt(m_cameraPosition,
			m_cameraTarget,
			m_cameraUp);

		//draw geometry

		drawTerrain();
		drawReflector();
		drawGeometry(false, m_camera->getCameraMatrix());
		//	m_lines->draw(m_camera, LineRenderer::Mode::LINES);
	}
}
