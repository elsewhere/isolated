#pragma once

#include "Light.h"
#include "../Globals.h"

namespace demorender
{
	struct ShadowMapParameters
	{
		int width = 1024;
		int height = 1024;
		float nearPlane = 1.f;
		float farPlane = 150.f;
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

		const ShadowMapParameters& getParams() { return m_params; }

		void debugDraw();

		std::vector<glm::mat4> getCubeTransforms() { return m_cubeTransforms;  }
	private:
		enum class Type
		{
			ORTHO,
			PERSPECTIVE,
			CUBE,
		};

		ShadowMapParameters m_params;
		Type m_type;
		Light m_light;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		std::vector<glm::mat4> m_cubeTransforms;

		std::unique_ptr<DepthMap> m_depthMap;



	};

}