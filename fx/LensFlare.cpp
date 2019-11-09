#include "LensFlare.h"
#include "../Globals.h"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtx/norm.hpp"

using namespace demorender;

namespace demofx
{
	LensFlare::LensFlare()
	{
		m_debugCamera = demorender::Camera(1.f, 1000.f, 45.f);
		m_lines = std::make_unique<demorender::LineRenderer>();

		LensDescription lensData;
		parseSystem(lensData.angenieux);// .nikon_28_75mm);
	}

	LensFlare::~LensFlare()
	{

	}

	void LensFlare::debugDraw()
	{
		const float offset = 100.f;
		m_debugCamera.lookAt(glm::vec3(offset, 0.f, 200.f), glm::vec3(offset, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		m_lines->clear();
		m_lines->startNewBatch();

//		for (int i = 0; i < 500; i++)
//		{
//			glm::vec3 p = Math::randVectSphere() * Math::randFloat() * 150.f;
//			m_lines->addPoint(p, glm::vec4(1.f));
//		}

//		m_lines->addPoint(glm::vec3(-40.f, -40.f, 0));
//		m_lines->addPoint(glm::vec3( 40.f, -40.f, 0));
//		m_lines->addPoint(glm::vec3( 40.f,  40.f, 0));
//		m_lines->addPoint(glm::vec3(-40.f,  40.f, 0));
//		m_lines->addPoint(glm::vec3(-40.f, -40.f, 0));

		for (auto& l : m_system)
		{
			debugDraw(l);
		}



		glDisable(GL_DEPTH_TEST);
		m_lines->draw(&m_debugCamera, LineRenderer::Mode::LINES);
	}

	void LensFlare::parseSystem(const std::vector<PatentFormat>& data)
	{
		float totalDistance = 0.f;
		m_system.clear();
		int count = 0;

		std::stringstream ss;
		for (int i = data.size() - 1; i >= 0; i--)
		{
			LensInterface lensPart;
			const PatentFormat& entry = data[i];

			totalDistance += entry.d;

			float left_ior = (i == 0) ? 1.f : data[i - 1].n;
			float right_ior = entry.n;

			lensPart.center = glm::vec3(0.f, 0.f, totalDistance - entry.r);
			lensPart.n = glm::vec3(left_ior, 1.f, right_ior);

			if (right_ior != 1.f)
			{
				min_ior = std::min(min_ior, right_ior);
				max_ior = std::max(max_ior, right_ior);
			}

			lensPart.sa = entry.h;
			lensPart.d1 = entry.c;
			lensPart.flat = (float)entry.f;
			lensPart.pos = totalDistance;

			lensPart.w = entry.w;

			ss.str("");
			ss << "added lens interface " << count << " center = " << Math::toString(lensPart.center) << " ior = " << Math::toString(lensPart.n) << "\n";
			m_system.push_back(lensPart);

			g_debug << ss.str();
			
			count++;
		}
		ss.str("");
		ss << "lens contains " << count << "pieces, min ior = " << min_ior << " max ior = " << max_ior << "\n";
		g_debug << ss.str();
	}

	void LensFlare::debugDraw(const LensInterface& i)
	{
		m_lines->startNewBatch();
		glm::vec3 pos = glm::vec3(i.center.z, 0.f, 0.f);

		//draw a line

		m_lines->addPoint(pos + glm::vec3(0.f, -30.f, 0.f));
		m_lines->addPoint(pos + glm::vec3(0.f,  30.f, 0.f));



	}

}