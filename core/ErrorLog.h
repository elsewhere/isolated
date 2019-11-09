#pragma once

#include <vector>
#include <fstream>
#include <windows.h>

namespace democore
{
	class ErrorLog
	{
	public:
		ErrorLog();
		~ErrorLog();

		void log(const std::string& str)
		{
			m_buffer.push_back(str);
		}

		template<typename T>
		void log(const std::string& name, T value)
		{
			std::stringstream ss;
			ss << name << " = " << value;
			log(ss.str());
		}

		void setColor(float r, float g, float b);
		void clear();
		void draw();

		std::vector<std::string>& getBuffer() { return m_buffer; }
	private:
		float m_r, m_g, m_b;
		std::vector<std::string> m_buffer;

	};
}
