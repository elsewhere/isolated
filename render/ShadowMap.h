#pragma once

#include "../Globals.h"

namespace demorender
{
	class ShadowMap
	{
	public:

		ShadowMap();
		~ShadowMap();

		void bind();
		void unbind();

		glm::mat4 getLightMatrix();
		int getDepthMapID();

		void debugDraw();

	private:

		std::unique_ptr<DepthMap> m_depthMap;



	};

}