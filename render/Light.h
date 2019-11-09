#pragma once

#include "../Globals.h"

namespace demorender
{
	class Light
	{
	enum Type
	{

	};
	public:

		Light();
		~Light();

		void setPosition(glm::vec3& position);
		glm::vec3 getPosition();

		void setColor(int hex);
		void setColor(glm::vec3& color);
		void setColor(float r, float b, float g);

		glm::vec3 getColor();
		

	private:

		glm::vec3 m_position;
		glm::vec3 m_color;

	};

}