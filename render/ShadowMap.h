#pragma once

#include "Light.h"
#include "../Globals.h"

namespace demorender
{
	struct ShadowMapParameters
	{
		int width = 1024;
		int height = 1024;
		bool cullFrontFaces = true;
		bool cubeMap = false;
	};

	class ShadowMap
	{
	public:

		ShadowMap(const ShadowMapParameters& params);
		~ShadowMap();

		void prepare(const demorender::Light& light);
		void unbind();

		glm::mat4 getLightMatrix();
		int getDepthMapID();

		void debugDraw();

	private:
		enum class Type
		{
			ORTHO,
			PERSPECTIVE,
		};

		ShadowMapParameters m_params;
		Type m_type;
		Light m_light;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		std::unique_ptr<DepthMap> m_depthMap;



	};

}