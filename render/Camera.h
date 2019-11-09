#pragma once

#include "../Globals.h"

namespace demorender
{
	class Camera
	{
	public:

		Camera(float nearPlane = 1.f, float farPlane = 100.f, float fov = 90.f);
		~Camera();

		void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));
		glm::vec3 getX() const;
		glm::vec3 getY() const;
		glm::vec3 getZ() const;
		glm::vec3 getT() const;

		void setPlanes(float nearPlane, float farPlane);
		float getFarPlane() const;
		float getNearPlane() const;
		void setFov(float fov);
		float getFov();

		glm::mat4 getCameraMatrix() const;
		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewMatrix() const; 

		void setForTexture(const std::string& texture);
		void reset();

	private:
		void calculateProjectionMatrix();
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::vec3 m_position;
		float m_fov;
		float m_nearPlane;
		float m_farPlane;
		

	};

}