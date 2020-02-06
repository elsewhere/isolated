#pragma once

#include "../Globals.h"

namespace demorender
{
	class Light
	{
	public:

		enum class Type
		{
			DIRECTIONAL,
			SPOT,
			POINT,
		};

		Light();
		~Light();

		void setPosition(const glm::vec3& position);
		void setTarget(const glm::vec3& target);
		void setUp(const glm::vec3& up);
		glm::vec3 getPosition() const;
		glm::vec3 getTarget() const;
		glm::vec3 getUp() const;

		void setColor(int hex);
		void setColor(const glm::vec3& color);
		void setColor(const glm::vec4& color);
		void setColor(float r, float b, float g);

		glm::vec3 getColor();
		
		Type getType() const;
		void setType(Type type);
	private:
		Type m_type;

		glm::vec3 m_position;
		glm::vec3 m_target;
		glm::vec3 m_up;

		glm::vec3 m_color;

	};

}