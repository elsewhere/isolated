#include "ShadowMap.h"
#include "../glm/gtx/transform.hpp"

namespace demorender
{
	ShadowMap::ShadowMap(const ShadowMapParameters& params):
		m_params(params)
	{

		TextureParameters textureParams;
		textureParams.m_type = params.cubeMap ? TextureParameters::DEPTH_CUBEMAP : TextureParameters::DEPTH;
		textureParams.m_width = m_params.width;
		textureParams.m_height = m_params.height;

		g_debug << "creating shadow map " << textureParams.m_width << "x" << textureParams.m_height << " " << (params.cubeMap ? "cube map" : "texture") << "\n";

		m_depthMap = std::make_unique<DepthMap>();
		m_depthMap->create(textureParams);

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

		if (m_params.cullFrontFaces)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

		}
	}

	void ShadowMap::unbind()
	{
		m_depthMap->unbind();

		if (m_params.cullFrontFaces)
		{
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
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