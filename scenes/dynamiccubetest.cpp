#include "DynamicCubeTest.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/CubemapRenderer.h"
#include "../render/LineRenderer.h"
#include "../render/ShadowMap.h"

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
	
	ShadowMapParameters params;

	params.width = 1024;
	params.height = 1024; 
	params.cubeMap = true;
	params.cullFrontFaces = true;
	
	m_shadowMap = std::make_unique<ShadowMap>(params);

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

	m_cubemapRenderer = std::make_unique<CubemapRenderer>();

}

void DynamicCubeTest::update()
{
	g_params->useNamespace("DynamicCubeTest");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	glm::vec3 cameraStart = glm::vec3(-30, 5.f, -40.f);
	glm::vec3 cameraEnd = glm::vec3(40, 10.f, 20.f);

	m_cameraPosition = cameraStart + (cameraEnd - cameraStart) * m_pos;

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
	if (shadowPass)
	{
		Shader& s = g_shaders->getShader("cubedepthonly");

		s.bind();
		s.setUniform1f("farplane", m_shadowMap->getParams().farPlane);

		s.setUniformMatrix4fv("shadowTransforms[0]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::POS_X));
		s.setUniformMatrix4fv("shadowTransforms[1]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::NEG_X));
		s.setUniformMatrix4fv("shadowTransforms[2]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::POS_Y));
		s.setUniformMatrix4fv("shadowTransforms[3]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::NEG_Y));
		s.setUniformMatrix4fv("shadowTransforms[4]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::POS_Z));
		s.setUniformMatrix4fv("shadowTransforms[5]", 1, GL_FALSE, (float *)&m_shadowMap->getCubeTransform(Cubemap::NEG_Z));

		m_thingMesh->bind(&s);
		for (auto t : m_things)
		{
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			m_thingMesh->draw();
		}
	}
	else
	{
		Shader& s = g_shaders->getShader("cubething");

		s.bind();
		m_thingMesh->bind(&s);
		s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&cameraMatrix); GL_DEBUG;
		s.setUniform3fv("lightPos", 1, (float *)&m_pointLight.getPosition()); GL_DEBUG;
		s.setUniform1f("farplane", m_shadowMap->getParams().farPlane);
		s.setUniform1i("shadowMap", 0);
		g_textures->bindCubemap(m_shadowMap->getDepthMapID(), GL_TEXTURE0);
		
		for (auto t : m_things)
		{
			s.setUniform4fv("color", 1, (float *)&t->color);
			s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&t->transform); GL_DEBUG;
			m_thingMesh->draw();
		}

		//draw point light	
/*
		glm::mat4 model = glm::translate(m_pointLight.getPosition()) * glm::scale(glm::vec3(1.f));;

		s.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
		s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&model); GL_DEBUG;

		m_thingMesh->bind(&s);
		m_thingMesh->draw();
		*/

		Shader& reflectionShader = g_shaders->getShader("cubereflection");
		reflectionShader.bind();
		m_thingMesh->bind(&reflectionShader);

		glm::mat4 modelMatrix = glm::mat4(1.f);
		modelMatrix *= glm::scale(glm::vec3(3.f));
		reflectionShader.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&cameraMatrix); GL_DEBUG;
		reflectionShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		reflectionShader.setUniform3fv("cameraPosition", 1, (float *)&glm::vec3(0.f)); GL_DEBUG;
		reflectionShader.setUniform1i("textureMap", 0);
		reflectionShader.setUniform1i("cubeMap", 1);

		g_textures->bindTexture("kivitesti", GL_TEXTURE0);
		g_textures->bindCubemap("dynamictest", GL_TEXTURE1);

		m_thingMesh->draw();

//		uniform sampler2D textureMap;
//		uniform samplerCube cubeMap;

	}
}

void DynamicCubeTest::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("shadowcube");
	s.bind();

	g_textures->bindTexture("kivitesti", GL_TEXTURE0);
	g_textures->bindCubemap(m_shadowMap->getDepthMapID(), GL_TEXTURE1);
	s.setUniform1i("texturemap", 0);
	s.setUniform1i("shadowMap", 1);
	s.setUniform3fv("lightPos", 1, (float *)&m_pointLight.getPosition());
	s.setUniform1f("farplane", m_shadowMap->getParams().farPlane); 

	s.setUniformMatrix4fv("cameraMatrix", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("modelMatrix", 1, false, (float *)&model);

	m_terrain->bind(&s);
	m_terrain->draw();

}
void DynamicCubeTest::debug()
{
	m_shadowMap->debugDraw();
}

void DynamicCubeTest::draw()
{
	g_params->useNamespace("DynamicCubeTest");

	//render shadow map
	m_shadowMap->prepare(m_pointLight);
	drawGeometry(true, glm::mat4(1.f));
	m_shadowMap->unbind();

	//render reflection 
	m_cubemapRenderer->setPosition(m_pointLight.getPosition(), m_camera->getNearPlane(), m_camera->getFarPlane());

	for (int i = 0; i < 6; i++)
	{
		Cubemap::Side side = static_cast<Cubemap::Side>(i);
		m_cubemapRenderer->bind("dynamictest", side);
		drawGeometry(false, m_cubemapRenderer->getTransform(side));
	}

	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	//draw geometry

	drawTerrain();
	drawGeometry(false, m_camera->getCameraMatrix());
//	m_lines->draw(m_camera, LineRenderer::Mode::LINES);

}
