#pragma once

#include "../Globals.h"

#define g_sync SyncManager::inst()

namespace democore
{
	class TriggerSystem;
	class Event;
	class BPM;

	class SyncManager
	{
		public:
			static SyncManager *inst();
			void init();
			void clear();
			void update();
			void clearFlags();

			//additional stuff 
			void addBeat(const std::string& name, BPM *bpm);
			void addTriggers(const std::string& name, TriggerSystem *triggers);
			void addEvent(const std::string& name, Event *e);

			//accessors
			BPM& beat(const std::string& name);
			TriggerSystem& triggers(const std::string& name);
			Event& event(const std::string& name);

			std::vector<Event*> getEvents();
			std::vector<TriggerSystem*> getTriggers();

		private:
			SyncManager();
			~SyncManager();

			static SyncManager *sm_instance;
			std::map<std::string, BPM*> m_beats;
			std::map<std::string, TriggerSystem*> m_triggers;
			std::map<std::string, Event*> m_events;


	};

}