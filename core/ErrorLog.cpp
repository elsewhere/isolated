#include "../Globals.h"
#include "ErrorLog.h"

namespace democore
{

	ErrorLog::ErrorLog()
	{
		m_r = 1.f;
		m_g = 1.f;
		m_b = 1.f;
		m_buffer.clear();
	}

	ErrorLog::~ErrorLog()
	{
	}

	void ErrorLog::clear()
	{
		m_buffer.clear();
	}

	void ErrorLog::setColor(float r, float g, float b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}
	void ErrorLog::draw()
	{
		int y = 10;
		int x = 10;
		for (auto s : m_buffer)
		{
			demorender::g_renderDebug->drawString(s, x, y, 0.35f, glm::vec3(m_r, m_g, m_b));
			y += 20;
		}

	}

}

