#include "Profiler.h"
#include "../Globals.h"

namespace democore
{

	Profiler::Profiler()
	{
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
			DWORD timestamp = timeGetTime();
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
			DWORD now = timeGetTime();
			DWORD then = m_profiling[name];

			g_debug << name << " took " << (now - then) << " ms\n";

			m_profiling.erase(name);
		}
	}
}

