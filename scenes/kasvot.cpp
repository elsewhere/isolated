#include "Kasvot.h"
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

Kasvot::KasvotParticles::KasvotParticles() :
	GPUParticleSystem(1024 * 1024)
{
	addLogicShaderAttribute({ "particlePosition", 3 });
	addLogicShaderAttribute({ "particleColor", 4 });
	addLogicShaderAttribute({ "particleEnergy", 1 });
	addLogicShaderAttribute({ "particleMaxEnergy", 1 });

	addRenderShaderAttribute({ "vertexPosition", 3 });
	addRenderShaderAttribute({ "vertexColor", 4 });
	addRenderShaderAttribute({ "vertexEnergy", 1 });
	addRenderShaderAttribute({ "vertexMaxEnergy", 1 });
}

Kasvot::KasvotParticles::~KasvotParticles()
{

}

void Kasvot::KasvotParticles::setInitialData()
{
	m_pInitialData = new float[m_particleCount * m_particleSize];

	float* dataptr = m_pInitialData;
	for (int i = 0; i < m_particleCount; i++)
	{
		glm::vec3 position = Math::randVectSphere() * 5.f;
		glm::vec4 color = glm::vec4(1.f);
		float energy = Math::randFloat(0.5f, 1.5f);

		writeData<glm::vec3>(&dataptr, position);
		writeData<glm::vec4>(&dataptr, color);
		writeData<float>(&dataptr, energy);
		writeData<float>(&dataptr, energy);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kasvot
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

void Kasvot::createMesh()
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

void Kasvot::init()
{
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	createMesh();

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();

	for (int i = 0; i < 300; i++)
	{
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}

	m_pSkybox = new demorender::Model();
	m_pSkybox->setMesh("cube");

	m_particles = std::make_unique<KasvotParticles>();
	m_particles->setShaders("effect_kasvotparticle", "effect_kasvotparticlerender");
	m_particles->setInitialData();
	m_particles->createBuffers();
}

void Kasvot::update()
{
	g_params->useNamespace("Kasvot");

	const float light_t = 0.5f + 0.5f * sinf(m_pos * 130.f);

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	const float a = m_pos * 20.f;

	const float radius = 20.f;
	m_cameraPosition = glm::vec3(sinf(a) * radius, 10.f, cosf(a) * radius);
	m_cameraTarget = glm::vec3(0.f);
	m_cameraTarget.y = 0.f;

	m_particles->startFrame();
	m_particles->addRenderShaderUniform("tex", 0);
	m_particles->addRenderShaderUniform("viewMatrix", m_camera->getViewMatrix());
	m_particles->addRenderShaderUniform("projectionMatrix", m_camera->getProjectionMatrix());
	m_particles->addRenderShaderUniform("modelMatrix", glm::mat4(1.f));

	m_particles->update();
}

void Kasvot::drawTerrain()
{
	m_particles->draw(m_camera);
}

void Kasvot::drawBackground()
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

void Kasvot::debug()
{
}

void Kasvot::draw()
{
	g_params->useNamespace("Kasvot");

	g_renderTargets->bindMain();

	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	drawBackground();
	drawTerrain();

	const float focus = 0.1f;

	g_postProcess->addRadialGlow( g_params->get<int>("glowiterations"), 
								  g_params->get<float>("glowspread"),
								  g_params->get<float>("glowexponent"),
								  g_params->get<float>("glowalpha"));

	//	g_postProcess->addRadial();
	//	g_postProcess->addLens(focus, m_camera);
	//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
