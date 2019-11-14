#pragma once

#include <map>
#include <windows.h>

/*
    A simple profiling class
*/
namespace democore
{

	class Profiler
	{
	public:		
		Profiler();
		~Profiler();

		void startProfile(const std::string& name);
		void endProfile(const std::string& name);

	private:
		std::map < std::string, DWORD> m_profiling;
	};
}
