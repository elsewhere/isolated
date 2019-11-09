#pragma once

#include "../Globals.h"

namespace demomath
{
	class BezierCurve : public Curve
	{
	public:
		BezierCurve();
		~BezierCurve();
		void destroy();

		//point = piste, tangent = suunta (sis‰lt‰‰ jo pisteen paikan)
		void begin();
		void addPoint(glm::vec3 &point, glm::vec3 &tangent);
		void end();
	    
		glm::vec3 getValue(float t);
		glm::vec3 getTangent(float t);

		glm::vec3 *points;
		glm::vec3 *tangents;

	private:
		std::vector<glm::vec3> insertedPoints;
		std::vector<glm::vec3> insertedTangents;
		int pcount;
	};
}