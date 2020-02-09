#pragma once

#include "../Globals.h"

namespace demorender
{
	class CubemapRenderer
	{

	public:

		CubemapRenderer();
		~CubemapRenderer();

		void setPosition(const glm::vec3& position, float nearPlane, float farPlane);
		const glm::mat4& getTransform(Cubemap::Side side);


		void bind(const std::string& name, Cubemap::Side side, bool clear = true);
		void unbind();


		/*
			for (all sides)
				set rendering to FBO 
				setup side projection etc
				render geometry

		
		
		*/

	private:

		std::vector<std::pair<glm::vec3, glm::vec3>> m_basis;
		std::vector<glm::mat4> m_transforms;

	};

}