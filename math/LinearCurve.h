#pragma once

#include "../Globals.h"

namespace demomath
{
	class LinearCurve : public Curve
	{
	public:
		LinearCurve();
		~LinearCurve();

		glm::vec3 getValue(float t);
		glm::vec3 getTangent(float t);

		void begin();
		void addPoint(glm::vec3 &point);
		void end();

	private:
		std::vector<glm::vec3> m_points;

	};
	}