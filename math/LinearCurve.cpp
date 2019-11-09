#include <string>
#include "linearcurve.h"

namespace demomath
{
	LinearCurve::LinearCurve()
	{
	}

	LinearCurve::~LinearCurve()
	{
	}

	glm::vec3 LinearCurve::getValue(float t)
	{
		int size = m_points.size();
		if (size > 1)
		{
			//there are at least two points on the curve

			int index1 = (int)floor(t * size);
			int index2 = index1 + 1;

			glm::vec3 &p1 = m_points[index1];
			glm::vec3 &p2 = m_points[index2];

			return glm::vec3(0, 0, 0);

		}
		else
		{
			return glm::vec3(0, 0, 0);
		}
	}

	glm::vec3 LinearCurve::getTangent(float t)
	{
		return glm::vec3(0, 0, 0);
	}

	void LinearCurve::begin()
	{
		m_points.clear();
	}

	void LinearCurve::end()
	{
	}

	void LinearCurve::addPoint(glm::vec3 &point)
	{
		m_points.push_back(point);
	}
}