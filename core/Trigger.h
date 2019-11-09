#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Globals.h"

namespace democore
{

	class Trigger
	{
	public:
		Trigger() {};
		~Trigger() {};

		int m_time;
		int m_length;
		float m_power;
	};

	class TriggerSystem
	{
	public:
		TriggerSystem()
			;
		~TriggerSystem();

		void addTrigger(Trigger &trigger);
		void getTrigger(int index, int& time, int &length, float &power);
		float getInterpolatedDistanceToNext();
		float getNormalizedValue();
		float getValue();
		int getPosition();
		int getCount();
		float getPower();

		std::string getName() { return m_name; };
		void setName(const std::string& name) { m_name = name; }
		std::vector<Trigger> getTriggers() { return m_triggers; }

	private:
		std::string m_name;
		std::vector<Trigger> m_triggers;



	};
}