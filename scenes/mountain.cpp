#include "mountain.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mountain
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace
{
	static const std::string skyboxTexture = "nebula";
}


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

void Mountain::createMesh()
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

void Mountain::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);


	createMesh();

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");
}

void Mountain::update()
{
	g_params->useNamespace("mountain");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	const float radius = 100.f;
	const float height = 70.f;

	const float a = m_pos * 2.f;

	glm::vec3 cameraStart = glm::vec3(0, 0, -240.f);
	glm::vec3 cameraEnd = glm::vec3(0, 0, 240.f);
	m_cameraPosition = glm::vec3(sinf(a), 0.f, cosf(a)) * radius;
	m_cameraPosition.y = height;

	m_cameraPosition += cameraStart + (cameraEnd - cameraStart) * m_pos;

	m_cameraTarget = m_cameraPosition * 0.000f;
	m_cameraTarget.y = 0.f;// ::vec3(0.f);
}

void Mountain::drawTerrain()
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

void Mountain::drawBackground()
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
void Mountain::draw()
{
	g_params->useNamespace("mountain");

	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

//	drawBackground();
	drawTerrain();

//	const float focus = 0.5f + 0.5f * sinf(m_pos * 150.f);
	const float focus = 0.1f;
	g_postProcess->addLens(focus, m_camera);
	std::stringstream ss;
	ss << "cam = " << Math::toString(m_cameraPosition) << " time = " << m_pos;
	g_screenText.log(ss.str());
//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
