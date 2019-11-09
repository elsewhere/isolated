#include "Light.h"
#include "../glm/gtx/transform.hpp"


namespace demorender
{
	Light::Light():m_position(glm::vec3(0.f))
		, m_color(glm::vec3(1.f))
	{
	}

	Light::~Light()
	{
	}

	void Light::setPosition(glm::vec3& position)
	{
		m_position = position;
	}
	glm::vec3 Light::getPosition()
	{
		return m_position;

	}

	void Light::setColor(int hex)
	{
		float b = (hex & 0xFF) / 255.f;
		float g = ((hex >> 8) & 0xFF) / 255.f;
		float r = ((hex >> 16) & 0xFF) / 255.f;

		setColor(r, g, b);
	}
	void Light::setColor(glm::vec3& color)
	{
		m_color = color;

	}
	void Light::setColor(float r, float b, float g)
	{
		m_color = glm::vec3(r, g, b);
	}

	glm::vec3 Light::getColor()
	{
		return m_color;
	}


}