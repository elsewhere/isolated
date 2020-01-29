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

		m_viewMatrix = glm::mat4(1.f);
	}

	ShadowMap::~ShadowMap()
	{
	}

	void ShadowMap::prepare(const demorender::Light& light)
	{
		m_depthMap->bind();

		m_light = light;
		if (m_light.getType() == Light::Type::DIRECTIONAL)
		{
			m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
		}
		else if (m_light.getType() == Light::Type::POINT)
		{

		}
	}

	void ShadowMap::unbind()
	{
		m_depthMap->unbind();
	}

	glm::mat4 ShadowMap::getLightMatrix()
	{
		const float nearplane = 1.f;
		const float farplane = 100.f;

		if (m_light.getType() == Light::Type::DIRECTIONAL)
		{
			const float size = 60.f;
			glm::mat4 projection = glm::ortho(-size, size, -size, size, nearplane, farplane);
			return projection * m_viewMatrix;
		}
		else if (m_light.getType() == Light::Type::POINT)
		{

		}
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