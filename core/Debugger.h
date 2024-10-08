#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <windows.h>

/*
    Debug class used for writing the log file. 

*/


namespace democore
{

	class Debugger
	{
	public:		
		Debugger();
		~Debugger();

		void flush()
		{
		   m_debugFile.flush();
		}

		template<class T>
		std::ofstream& operator<<(const T &str)
		{
#ifdef _DEBUG
			printDate();
			m_debugFile << str;
			
			m_debugFile.flush();
#endif
			return m_debugFile;
		}

		template<class T>
		std::ofstream& operator + (const T &str)
		{
#ifdef _DEBUG
			m_debugFile << str << std::endl;
			m_debugFile.flush();
#endif
			return m_debugFile;
		}


	private:
		void printDate();
		std::ofstream m_debugFile;
	};
}
