#include "Camera.h"
#include "../glm/gtx/transform.hpp"

namespace demorender
{
	Camera::Camera(float nearPlane, float farPlane, float fov):
	m_nearPlane(nearPlane),
	m_farPlane(farPlane),
	m_fov(fov)
	{
		calculateProjectionMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{		
		m_viewMatrix = glm::lookAt(position, target, up);
	}
	glm::vec3 Camera::getX() const
	{
		return glm::vec3(glm::inverse(m_viewMatrix)[0]);
	}
	glm::vec3 Camera::getY() const
	{
		return glm::vec3(glm::inverse(m_viewMatrix)[1]);
	}
	glm::vec3 Camera::getZ() const
	{
		return glm::vec3(glm::inverse(m_viewMatrix)[2]);
	}
	glm::vec3 Camera::getT() const
	{
		return glm::vec3(glm::inverse(m_viewMatrix)[3]);
	}

	void Camera::setPlanes(float nearPlane, float farPlane)
	{
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		calculateProjectionMatrix();
	}
	void Camera::setFov(float fov)
	{
		m_fov = fov;
		calculateProjectionMatrix();
	}
	float Camera::getFov()
	{
		return m_fov;
	}
	float Camera::getFarPlane() const
	{
		return m_farPlane;
	}
	float Camera::getNearPlane() const
	{
		return m_nearPlane;
	}


	void Camera::setForTexture(const std::string& texture)
	{
		m_projectionMatrix = glm::perspective(m_fov, 1.f, m_nearPlane, m_farPlane);
	}
	void Camera::reset()
	{
		calculateProjectionMatrix();
	}

	void Camera::calculateProjectionMatrix()
	{
		m_projectionMatrix = glm::perspective(m_fov, democore::g_system->getAspectRatio(), m_nearPlane, m_farPlane);
	}

	glm::mat4 Camera::getCameraMatrix() const
	{
		return m_projectionMatrix * m_viewMatrix;
	}
	glm::mat4 Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
	glm::mat4 Camera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

}