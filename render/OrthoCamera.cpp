#include "OrthoCamera.h"
#include "../glm/gtx/transform.hpp"


namespace demorender
{
	OrthoCamera::OrthoCamera(float minX, float maxX, float minY, float maxY, float nearPlane, float farPlane):
	m_nearPlane(nearPlane),
	m_farPlane(farPlane),
	m_minX(minX),
	m_maxX(maxX),
	m_minY(minY),
	m_maxY(maxY)
	{
		calculateProjectionMatrix();
	}

	OrthoCamera::~OrthoCamera()
	{
	}

	void OrthoCamera::setDimensions(float minX, float maxX, float minY, float maxY)
	{
		m_minX = minX;
		m_maxX = maxX;
		m_minY = minY;
		m_maxY = maxY;

		calculateProjectionMatrix();
	}

	void OrthoCamera::setPlanes(float nearPlane, float farPlane)
	{
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		calculateProjectionMatrix();
	}
	float OrthoCamera::getFarPlane() const
	{
		return m_farPlane;
	}
	float OrthoCamera::getNearPlane() const
	{
		return m_nearPlane;
	}

	void OrthoCamera::calculateProjectionMatrix()
	{
		m_projectionMatrix = glm::ortho(m_minX, m_maxX, m_minY, m_maxY);
	}
	
	glm::mat4 OrthoCamera::getCameraMatrix() const
	{
		return m_projectionMatrix;
	}
}