#include "trigger.h"

namespace democore
{

	TriggerSystem::TriggerSystem()
	{
		m_triggers.clear();
	}

	TriggerSystem::~TriggerSystem()
	{
	}

	void TriggerSystem::addTrigger(Trigger &trigger)
	{
		m_triggers.push_back(trigger);
		sort(m_triggers.begin(), m_triggers.end(), [](auto a, auto b) { return a.m_time < b.m_time; });
	}

	float TriggerSystem::getNormalizedValue()
	{
		float value = 0.0;
		int time = g_system->getTime();

		for (auto& t : m_triggers)
		{
			if (time >= t.m_time && time < t.m_time + t.m_length)
			{
				//1 kun time == t.time
				//0 kun time == t.time + length
				float pos = 1.0f - (time - t.m_time) / (float)t.m_length;
				value += pos;
			}
		}
		//clamp
		//value = min(value, 1);
		return value;
	}

	float TriggerSystem::getValue()
	{
		float value = 0.0;
		int time = g_system->getTime();

		for (auto& t : m_triggers)
		{
			if (time >= t.m_time && time < t.m_time + t.m_length)
			{
				//1 kun time == t.time
				//0 kun time == t.time + length
				float pos = 1.0f - (time - t.m_time) / (float)t.m_length;
				value += pos * t.m_power;
			}
		}
		//clamp
		//value = min(value, 1);
		return value;
	}

	float TriggerSystem::getPower()
	{
		int time = g_system->getTime();

		for (auto& t : m_triggers)
		{
			if (time >= t.m_time && time < t.m_time + t.m_length)
			{
				return t.m_power;
			}
		}
		return 0.f;
	}

	void TriggerSystem::getTrigger(int index, int& time, int &length, float &power)
	{
		int size = getCount();
		if (index < 0)
		{
			g_debug << "trying to get trigger that has the index " << index << std::endl;
			return;
		}
		if (index >= size)
		{
			g_debug << "trying to get trigger " << index << " from the system that has only " << size << " triggers" << std::endl;
			return;
		}
		time = m_triggers[index].m_time;
		length = m_triggers[index].m_length;
		power = m_triggers[index].m_power;
	}


	int TriggerSystem::getPosition()
	{
		int time = g_system->getTime();

		int count = 0;
		int size = (int)m_triggers.size();

		for (auto& t : m_triggers)
		{
			if (time < t.m_time)
			{
				break;
			}
			count++;
		}
		return count;

	}

	int TriggerSystem::getCount()
	{
		return m_triggers.size();
	}
}