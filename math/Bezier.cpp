#include "Bezier.h"

namespace demomath
{
	BezierCurve::BezierCurve()
	{
		points = 0;
		tangents = 0;
	}

	BezierCurve::~BezierCurve()
	{
		this->destroy();
	}

	void BezierCurve::destroy()
	{
		if (points != 0)
		{
			delete [] points;
		}
		if (tangents != 0)
		{
			delete [] tangents;
		}
	}
	void BezierCurve::begin()
	{
		this->destroy();

	}
	void BezierCurve::addPoint(glm::vec3 &point, glm::vec3 &tangent)
	{
		this->insertedPoints.push_back(point);
		this->insertedTangents.push_back(tangent);
	}

	void BezierCurve::end()
	{
		//destroy() kutsuttu jo aikaisemmin
		this->pcount = this->insertedPoints.size();

		this->points = new glm::vec3[this->pcount];
		this->tangents = new glm::vec3[this->pcount];

		std::vector<glm::vec3>::iterator i;

		int pointoffset = 0;
		for (i = this->insertedPoints.begin(); i != this->insertedPoints.end(); i++)
		{
			glm::vec3 p = *i;
			this->points[pointoffset++] = p;
		}

		pointoffset = 0;
		for (i = this->insertedTangents.begin(); i != this->insertedTangents.end(); i++)
		{
			glm::vec3 p = *i;
			this->tangents[pointoffset++] = p;
		}
		this->insertedPoints.clear();
		this->insertedTangents.clear();
	}


	glm::vec3 BezierCurve::getTangent(float pos)
	{
		return glm::vec3(0,0,0);
	}
	glm::vec3 BezierCurve::getValue(float pos)
	{
	//    float val = (pos * (pcount-3)) + 1;
	//    const int index = ((int)floorf(pos * (pcount-3))) + 1;

		if (pos > 1.0000f)
		{
			return glm::vec3(0, 0, 0);
		}
		float val = pos * (pcount-1);
		const int index = (int)floorf(pos * (pcount-1));
	    
		float t = val - index;

		float t2 = t*t;
		float t3 = t2*t;

		glm::vec3 p0 = points[index];
		glm::vec3 p1 = tangents[index];
		glm::vec3 p2 = tangents[index + 1];
		glm::vec3 p3 = points[index + 1];

		glm::vec3 result = p0*(1.f-t)*(1.f-t)*(1.f-t) + p1*3.f*t*(1.f-t)*(1.f-t) + p2*3.f*t2*(1.f-t)+p3*t3; 
		return result;

	}
}
