#include "ShadowMap.h"
#include "../glm/gtx/transform.hpp"

namespace demorender
{
	ShadowMap::ShadowMap()
	{
		TextureParameters params;
		params.m_type = TextureParameters::DEPTH;
		params.m_width = 1024;
		params.m_height = 1024;

		g_debug << "creating shadow map\n";
		m_depthMap = std::make_unique<DepthMap>();
		m_depthMap->create(params);
	}

	ShadowMap::~ShadowMap()
	{
	}

	void ShadowMap::bind()
	{
		m_depthMap->bind();
	}

	void ShadowMap::unbind()
	{
		m_depthMap->unbind();
	}

	glm::mat4 ShadowMap::getLightMatrix()
	{

//		static float t = 0.f;
//		t += 0.001f;

		const float nearplane = 1.f;
		const float farplane = 100.f;

		const float size = 50.f;
		glm::mat4 projection = glm::ortho(-size, size, -size, size, nearplane, farplane); 

		glm::vec3 lightPosition = glm::vec3(2.f, 10.f, 0.f);// glm::vec3(10.f, -20.f, 5.f);
//		glm::vec3 lightPosition = glm::vec3(3 * sinf(t), 10.f, 0.f);// glm::vec3(10.f, -20.f, 5.f);
		glm::vec3 lightTarget = glm::vec3(0.f);
		glm::vec3 lightUp = glm::vec3(1.f, 0.f, 0.f);

		glm::mat4 view = glm::lookAt(lightPosition, lightTarget, lightUp);

		return projection * view;

	}

	int ShadowMap::getDepthMapID()
	{
		return m_depthMap->getID();
	}

	void ShadowMap::debugDraw()
	{
		g_renderDebug->drawDepthTextureOrtho(m_depthMap->getID(), 512 + 256, 256, 512.f);

	}
}