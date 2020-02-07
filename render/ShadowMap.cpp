#include "ShadowMap.h"
#include "CubemapRenderer.h"
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

		m_cubemapRenderer = std::make_unique<CubemapRenderer>();

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
		auto type = m_light.getType();

		switch (type)
		{
			case Light::Type::DIRECTIONAL:
			{
				m_type = Type::ORTHO;

				const float size = m_params.orthoSize;

				m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
				m_projectionMatrix = glm::ortho(-size, size, -size, size, m_params.nearPlane, m_params.farPlane);
			}  break;

			case Light::Type::SPOT:
			{
				m_type = Type::PERSPECTIVE;

				m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
				m_projectionMatrix = glm::perspective(90.f, 1.f, m_params.nearPlane, m_params.farPlane);
			}

			case Light::Type::POINT:
			{
				if (m_params.cubeMap)
				{
					m_type = Type::CUBE;
					m_cubemapRenderer->setPosition(m_light.getPosition(), m_params.nearPlane, m_params.farPlane);
				}
				else
				{
					m_type = Type::PERSPECTIVE;

					m_viewMatrix = glm::lookAt(m_light.getPosition(), m_light.getTarget(), m_light.getUp());
					m_projectionMatrix = glm::perspective(90.f, 1.f, m_params.nearPlane, m_params.farPlane);
				}
			}
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
		if (m_params.cubeMap)
		{
			return glm::mat4(1.f); 
		}
		return m_projectionMatrix * m_viewMatrix;
	}

	glm::mat4 ShadowMap::getCubeTransform(Cubemap::Side side)
	{
		return m_cubemapRenderer->getTransform(side);
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
			} break;

			case Type::CUBE:
			{


			} break;

		}

	}
}