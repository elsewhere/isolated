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
		m_type = Type::ORTHO;
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
			m_type = Type::ORTHO;

			//TODO: fix these
			const float nearplane = 1.f;
			const float farplane = 100.f;
			const float size = 60.f;

			m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
			m_projectionMatrix = glm::ortho(-size, size, -size, size, nearplane, farplane);
		}
		else if (m_light.getType() == Light::Type::POINT)
		{
			m_type = Type::PERSPECTIVE;

			const float nearplane = 1.f;
			const float farplane = 100.f;

			m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
			m_projectionMatrix = glm::perspective(90.f, 1.f, nearplane, farplane);
		}
	}

	void ShadowMap::unbind()
	{
		m_depthMap->unbind();
	}

	glm::mat4 ShadowMap::getLightMatrix()
	{
		return m_projectionMatrix * m_viewMatrix;
	}

	int ShadowMap::getDepthMapID()
	{
		return m_depthMap->getID();
	}

	void ShadowMap::debugDraw()
	{
		switch (m_type)
		{
			case Type::ORTHO:
			{
				g_renderDebug->drawDepthTextureOrtho(m_depthMap->getID(), 512 + 256, 256, 512.f);
			} break;

			case Type::PERSPECTIVE:
			{
//				g_renderDebug->drawDepthTexture(m_depthMap->getID(), 512 + 256, 256, 512.f);
			}

		}

	}
}