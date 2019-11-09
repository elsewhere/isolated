#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Globals.h"

namespace democore
{

	class Event
	{
	public:
		Event() {};
		~Event() {};

		float getValue();
		bool hasPassed();
		bool isOver();

		int m_time;
		int m_length;
		std::string m_name;
	};
}

