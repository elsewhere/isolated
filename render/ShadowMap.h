#pragma once

#include "../Globals.h"

namespace demorender
{
	class Light;

	class ShadowMap
	{
	public:

		ShadowMap();
		~ShadowMap();

		void prepare(const demorender::Light& light);
		void unbind();

		glm::mat4 getLightMatrix();
		int getDepthMapID();

		void debugDraw();

	private:
		demorender::Light m_light;
		glm::mat4 m_viewMatrix;

		std::unique_ptr<DepthMap> m_depthMap;



	};

}