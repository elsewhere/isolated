#include "Light.h"
#include "../glm/gtx/transform.hpp"


namespace demorender
{
	Light::Light(): m_position(glm::vec3(0.f))
		, m_color(glm::vec3(1.f))
		, m_type(Type::DIRECTIONAL)
		, m_target(glm::vec3(0.f, 0.f, 0.f))
		, m_up(glm::vec3(0.f, 1.f, 0.f))
	{
	}

	Light::~Light()
	{
	}

	Light::Type Light::getType() const
	{
		return m_type;
	}

	void Light::setType(Type type)
	{
		m_type = type;
	}
	
	void Light::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}
	glm::vec3 Light::getPosition() const
	{
		return m_position;
	}
	void Light::setUp(const glm::vec3& up)
	{
		m_up = up;
	}
	glm::vec3 Light::getUp() const
	{
		return m_up;
	}

	void Light::setTarget(const glm::vec3& target)
	{
		m_target = target;
	}
	glm::vec3 Light::getTarget() const
	{
		return m_target;
	}

	void Light::setColor(int hex)
	{
		float b = (hex & 0xFF) / 255.f;
		float g = ((hex >> 8) & 0xFF) / 255.f;
		float r = ((hex >> 16) & 0xFF) / 255.f;

		setColor(r, g, b);
	}
	void Light::setColor(const glm::vec3& color)
	{
		m_color = color;
	}

	void Light::setColor(const glm::vec4& color)
	{
		m_color = glm::vec3(color.x, color.y, color.z);
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