#include "catmullrom.h"

namespace demomath
{

	CatmullRomCurve::CatmullRomCurve()
	{
		this->points = 0;
		this->pcount = 0;
	}

	CatmullRomCurve::~CatmullRomCurve()
	{
		this->destroy();
	}

	void CatmullRomCurve::destroy()
	{
		this->pcount = 0;
		if (this->points != 0)
		{
			delete [] this->points;
			this->points = 0;
		}
	}

	CatmullRomCurve *CatmullRomCurve::clone()
	{
		CatmullRomCurve *temp = new CatmullRomCurve();
		temp->set(this);
		return temp;
	}

	void CatmullRomCurve::set(CatmullRomCurve *src)
	{
		this->destroy();
		this->pcount = src->pcount;
		this->points = new glm::vec3[this->pcount];

		for (int i = 0; i < this->pcount; i++)
		{
			this->points[i] = src->points[i];
		}

	}

	void CatmullRomCurve::begin()
	{
		this->destroy();
	}

	void CatmullRomCurve::addPoint(glm::vec3 &point)
	{
	   this->insertedPoints.push_back(point);
	}

	void CatmullRomCurve::reverse()
	{
		if (this->pcount <= 0 || !this->points)
		{
			return;
		}

		//ugly
		glm::vec3* temp = new glm::vec3[this->pcount];
		for (int i = 0; i < this->pcount; i++)
		{
			temp[i] = this->points[this->pcount - i - 1];
		}
		delete [] this->points;
		this->points = temp;


	}
	void CatmullRomCurve::end()
	{
		//destroy() kutsuttu jo aikaisemmin
		this->pcount = this->insertedPoints.size();

		this->points = new glm::vec3[this->pcount];

		std::vector<glm::vec3>::iterator i;

		int pointoffset = 0;
		for (i = this->insertedPoints.begin(); i < this->insertedPoints.end(); i++)
		{
			glm::vec3 &p = *i;
			this->points[pointoffset++] = p;
		}

		this->insertedPoints.clear();
	}

	void CatmullRomCurve::arcLengthParametrize(int newSplinePoints, int samples)
	{
		//lasketaan splinin pituus

		float l = 0.0f;
		float t = 0.0f;

		glm::vec3 prev = this->getValue(0.0f);

		for (t = 0.0f; t < 1.0f; t += 1.0f / samples)
		{
			glm::vec3 cur = this->getValue(t);
			glm::vec3 diff = cur - prev;

			l += diff.length();
			prev = cur;
		}

		if (newSplinePoints == 0)
		{
			newSplinePoints = this->pcount;
		}


		float segmentLength = l / newSplinePoints;
		float cumlength = 0.0f;
		prev = this->getValue(0.0f);

		t = 0.0f;

		CatmullRomCurve *uusi = new CatmullRomCurve();
		uusi->begin();

		for (int i = 0; i < newSplinePoints; i++)
		{
			while (cumlength < segmentLength)
			{
				//mennään spliniä eteenpäin
				glm::vec3 cur = this->getValue(t);
				float d = glm::distance(cur, prev);
				cumlength += d;

				prev = cur;
				t += 1.0f / samples;
			}
			//laitetaan uusi piste
			glm::vec3 p = this->getValue(t);
			uusi->addPoint(p);

			cumlength -= segmentLength;
		}

		uusi->end();
		this->destroy();
		this->set(uusi);




	}


	glm::vec3 CatmullRomCurve::getValue(float pos)
	{

		if (this->points == 0)
		{
			return glm::vec3(0, 0, 0);
		}
		if (this->pcount < 4)
		{
			return glm::vec3(0, 0, 0);
		}

		//t = 0 -> index = 1
		//t = 1.0f -> index = count - 2

		if (pos > 1.000f)
		{
			return glm::vec3(0, 0, 0);
		}
		if (pos < 0.000f)
		{
			return glm::vec3(0, 0, 0);
		}

		float val = (pos * (pcount-3)) + 1;
		const int index = ((int)floorf(pos * (this->pcount-3))) + 1;

		float t = val - index;

		float t2 = t*t;
		float t3 = t2*t;

		glm::vec3& p0 = this->points[index - 1];
		glm::vec3& p1 = this->points[index];
		glm::vec3& p2 = this->points[index + 1];
		glm::vec3& p3 = this->points[index + 2];

		glm::vec3 result = ((p1*2.f) + (p2 - p0) * t + (p0*2.f - p1 * 5.f + p2*4.f - p3)*t2 + (p1*3.f - p0- p2*3.f + p3) * t3)*0.5f;
		return result;

	}

	glm::vec3 CatmullRomCurve::getTangent(float pos)
	{
		return glm::vec3(0, 0, 0);
	}
}