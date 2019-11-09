#pragma once

#include "../Globals.h"

namespace demorender
{
	class OrthoCamera
	{
	public:

		OrthoCamera(float minX, float maxX, float minY, float maxY, float nearPlane = 1.f, float farPlane = 100.f);
		~OrthoCamera();

		void setPlanes(float nearPlane, float farPlane);
		float getFarPlane() const;
		float getNearPlane() const;

		void setDimensions(float minX, float maxX, float minY, float maxY);

		glm::mat4 getCameraMatrix() const;

	private:

		float m_minX, m_maxX, m_minY, m_maxY;
		void calculateProjectionMatrix();
		glm::mat4 m_projectionMatrix;
		glm::vec3 m_position;
		float m_nearPlane;
		float m_farPlane;
		

	};

}