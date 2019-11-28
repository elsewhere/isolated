#include "Profiler.h"
#include "../Globals.h"

namespace democore
{

	Profiler::Profiler()
	{
		QueryPerformanceFrequency(&m_frequency);
	}

	Profiler::~Profiler()
	{
	}

	void Profiler::startProfile(const std::string& name)
	{
		if (m_profiling.find(name) != m_profiling.end())
		{
			g_debug << "Profiler event " << name << " already exists\n";
		}
		else
		{
			LARGE_INTEGER timestamp;
			QueryPerformanceCounter(&timestamp);
			m_profiling[name] = timestamp;
		}

	}
	void Profiler::endProfile(const std::string& name)
	{
		if (m_profiling.find(name) == m_profiling.end())
		{
			g_debug << "Profiler event " << name << " does not exist\n";
		}
		else
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			LARGE_INTEGER then = m_profiling[name];

			LARGE_INTEGER elapsed;
			elapsed.QuadPart = now.QuadPart - then.QuadPart;
			elapsed.QuadPart *= 1000;
			elapsed.QuadPart /= m_frequency.QuadPart;

			g_debug << name << " took " << elapsed.QuadPart << " ms\n";

			m_profiling.erase(name);
		}
	}
}

