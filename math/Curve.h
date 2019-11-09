#pragma once

/*
	Base class for different curves 

*/

#include "../Globals.h"

namespace demomath
{
	class Curve
	{
	public:
		Curve() {};
		virtual ~Curve() {};
		//getValue() returns a point on the curve, t is normalized (goes from 0 to 1)
		virtual glm::vec3 getValue(float t) = 0;
		//getTangent() returns the tangent to the point on the curve, t is normalized
		virtual glm::vec3 getTangent(float t) = 0;

	};
}