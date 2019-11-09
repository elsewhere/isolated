#include "SyncManager.h"

namespace democore
{
	SyncManager *SyncManager::sm_instance = nullptr;

	SyncManager* SyncManager::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new SyncManager();
		}
		return sm_instance;

	}

	void SyncManager::init()
	{
		clear();
	}
	void SyncManager::clear()
	{
		m_beats.clear();
		m_triggers.clear();
		m_events.clear();
	}

	void SyncManager::update()
	{
		//update BPM counters
		for (auto& beat : m_beats)
		{
			beat.second->update();
		}
	}

	void SyncManager::clearFlags()
	{
		for (auto& beat : m_beats)
		{
			beat.second->clearFlags();
		}
	}
	void SyncManager::addBeat(const std::string& name, BPM *bpm)
	{
		m_beats[name] = bpm;
		m_beats[name]->initValues();
	}
	BPM &SyncManager::beat(const std::string& name)
	{
		return *m_beats[name];
	}

	void SyncManager::addTriggers(const std::string& name, TriggerSystem *triggers)
	{
 		m_triggers[name] = triggers;
	}
	TriggerSystem& SyncManager::triggers(const std::string& name)
	{
		if (m_triggers.find(name) == m_triggers.end())
		{
			g_debug << "Cannot find triggers " << name << "!" << std::endl;
		}
		return *m_triggers[name];
	}

	void SyncManager::addEvent(const std::string& name, Event *e)
	{
		m_events[name] = e;
	}
	Event& SyncManager::event(const std::string& name)
	{
		if (m_events.find(name) == m_events.end())
		{
			g_debug << "Cannot find event " << name << "!" << std::endl;
		}
		return *m_events[name];
	}

	std::vector<Event*> SyncManager::getEvents()
	{
		std::vector<Event*> events;

		for (auto it = m_events.begin(); it != m_events.end(); it++)
		{
			events.push_back((*it).second);
		}

		return events;
	}

	std::vector<TriggerSystem*> SyncManager::getTriggers()
	{
		std::vector<TriggerSystem*> triggers;

		for (auto it = m_triggers.begin(); it != m_triggers.end(); it++)
		{
			triggers.push_back((*it).second);
		}

		return triggers;
	}

	SyncManager::SyncManager()
	{
	}

	SyncManager::~SyncManager()
	{
	}

}