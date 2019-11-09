#pragma once

#include <vector>
#include <fstream>
#include <windows.h>

/*
    Debug class used for writing the log file. 

*/
namespace democore
{

	class Debugger
	{
	public:
		Debugger(bool enabled = true);
		~Debugger();

		void setEnabled(bool enabled);
		void flush()
		{
		   m_debugFile.flush();
		}

		template<class T>
		std::ofstream& operator<<(const T &str)
		{
			printDate();
			m_debugFile << str;
			m_debugFile.flush();
			return m_debugFile;
		}

		template<class T>
		std::ofstream& operator + (const T &str)
		{
			m_debugFile << str << std::endl;
			m_debugFile.flush();
			return m_debugFile;
		}

	private:
		void printDate();
		std::ofstream m_debugFile;

	};
}
