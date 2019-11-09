#pragma once

#include <vector>
#include "../Globals.h"

/*
    Catmull-Rom spline class.
*/

namespace demomath
{

	class CatmullRomCurve : public Curve
	{
	public:
		CatmullRomCurve();
		~CatmullRomCurve();

		//palauttaa kopion
		CatmullRomCurve *clone();

		void begin();
		void addPoint(glm::vec3 &point);
		void end();

		//parametrisoi kaaren pituuden mukaan (laittaa pisteet tasav�lein)
		//tuhoaa vanhan splinen, ts. osoittimet taulukkoon hajoavat
		//newSplinePoints on uuden splinin pisteiden m��r� (nollalla k�ytt�� alkuper�ist� pistem��r��)
		//samples on splinist� otettavien n�ytteiden m��r� 
		void arcLengthParametrize(int newSplinePoints = 0, int samples = 1000);

		glm::vec3 getValue(float t);
		glm::vec3 getTangent(float t);

		void reverse();

		glm::vec3 *getPoints() { return this->points; };
		int getPointCount() { return this->pcount; };

	private:
		int pcount;
		glm::vec3 *points;

		void destroy();
		void set(CatmullRomCurve *src);

		std::vector<glm::vec3> insertedPoints;


	};
}