#include "midaftann.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../fx/Cloth.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Midaftann
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	static const std::string skyboxTexture = "fjaraenv";
}

void Midaftann::createMesh()
{
	if (m_pMesh)
		delete m_pMesh;

	const int zres = 100;
	const int xres = 100;
	const float scale = 250.f;

	MeshBuilder *builder = new MeshBuilder();
	builder->generatePlane(xres, zres, scale);
	m_pMesh = builder->getMesh();
}

void Midaftann::init()
{
	m_camera = new demorender::Camera(1.f, 1500.f, 45.f);


	createMesh();

	m_pFlag = new demofx::Cloth();

	glm::mat4 pointTransform = glm::scale(glm::vec3(2.f, 1.f, 1.f));
	m_pFlag->init(50, 50, pointTransform);

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");
}

void Midaftann::update()
{
	g_params->useNamespace("midaftann");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	m_cameraPosition = g_params->getVector3("cameraposition");
	m_cameraTarget = g_params->getVector3("cameratarget");

	m_pFlag->setWindParams(
		g_params->getFloat("windspeed"),
		g_params->getFloat("windmagnitude"),
		g_params->getFloat("winddensity"));
	m_pFlag->setClothParams(
		g_params->getFloat("damping"),
		g_params->getFloat("gravity"));

	m_pFlag->update(g_params->getFloat("dt"));

}

void Midaftann::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("effect_terrain");
	s.bind();

	g_textures->bindTexture("heightmap1", GL_TEXTURE0);
	g_textures->bindTexture("kivitesti", GL_TEXTURE1);
	s.setUniform1f("time", m_pos);
	s.setUniform1i("heightmap", 0);
	s.setUniform1i("stone", 1);
	s.setUniform1f("terrainScale", 1.2f);
	s.setUniform1f("terrainHeightScale", 24.f);
	s.setUniformMatrix4fv("camera", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("model", 1, false, (float *)&model);

	m_pMesh->bind(&s);
	m_pMesh->draw();
}

void Midaftann::drawBackground()
{
	Shader &s = g_shaders->getShader("skybox");
	s.bind();
	m_pSkybox->getMesh()->setStreamFlags(Mesh::VERTEX_STREAM);
	m_pSkybox->bind(&s);

	g_textures->bindCubemap(skyboxTexture, GL_TEXTURE0); GL_DEBUG;
	s.setUniform1i("tex", 0); GL_DEBUG;
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&m_camera->getCameraMatrix()); GL_DEBUG;

	glm::mat4 skyboxTrans = glm::scale(glm::vec3(790.f));
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&skyboxTrans); GL_DEBUG;
	glDrawArrays(GL_TRIANGLES, 0, 36); GL_DEBUG;
}
void Midaftann::draw()
{
	g_params->useNamespace("midaftann");

	g_renderTargets->bindMain();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); //TODO: refactor away
	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	drawBackground();
//	drawTerrain();

	glm::vec3 flagPos = g_params->getVector3("flagpos");// glm::vec3(0.f, 0.f, 0.f);
	glm::mat4 flagModelMatrix = glm::translate(flagPos) * glm::scale(glm::vec3(150.f));
	m_pFlag->draw(m_camera, flagModelMatrix);

//	const float focus = 0.5f + 0.5f * sinf(m_pos * 150.f);
//	const float focus = 0.1f;
//	g_postProcess->addLens(focus, m_camera);
//	stringstream ss;
//	ss << "cam = " << Math::toString(m_cameraPosition) << " time = " << m_pos;
//	g_screenText.log(ss.str());
//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
