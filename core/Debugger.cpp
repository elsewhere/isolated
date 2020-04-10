#include "Debugger.h"


namespace democore
{

	Debugger::Debugger()
	{
#ifdef _DEBUG
		m_debugFile.open("log.txt");
#endif
	}

	Debugger::~Debugger()
	{
#ifdef _DEBUG
	   m_debugFile.close();
#endif
	}

	void Debugger::printDate()
	{
#ifdef _DEBUG
	   SYSTEMTIME t;
	   GetSystemTime(&t);
	   m_debugFile << t.wYear << "-";
	   if(t.wMonth < 10)m_debugFile << "0";
	   m_debugFile << t.wMonth << "-";
	   if(t.wDay < 10)m_debugFile << "0";
	   m_debugFile << t.wDay << " ";
	   if(t.wHour < 10)m_debugFile << "0";
	   m_debugFile << t.wHour << ":";
	   if(t.wMinute < 10)m_debugFile << "0";
	   m_debugFile << t.wMinute << ":";
	   if(t.wSecond < 10)m_debugFile << "0";
	   m_debugFile << t.wSecond << "     ";
#endif
	}


}

