#pragma once

#include "../Globals.h"

/*
    A class for beatsyncing.

*/

namespace democore
{
	class BPM
	{
	public:
		BPM();
		~BPM();
		void init(float value, int offset);
		void initValues();
		void clearFlags();

		std::string getName() { return m_name; }
		void setName(const std::string& name) { m_name = name; }
		void setBPM(float bpm) { m_value = bpm; };
		void setOffset(int offset) { m_offset = offset; };

		//called once per frame, sets the internals    
		void update();
		//returns the distance to the last beat, 1 = on the beat, 0 = the next beat
		float getValue();
		//how many beats have we hit
		int getBeatcount();
		//returns true if the beat happened on this frame (could be used for strobes or something)
		bool getChanged();


	private:
		float m_value;
		int m_offset;
		int m_framecount;
		int m_lastBeats;
		int m_previousbeatdist;
		bool m_changed;
		float m_beat;
		std::string m_name;


	};
}


