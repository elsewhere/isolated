#include "bpm.h"

namespace democore
{
	BPM::BPM()
	{
	}
	BPM::~BPM()
	{
		m_framecount = 0;
		m_value = 0;
		m_offset = 0;
		m_previousbeatdist = 0;
		m_beat = 0.0f;

	}

	//  value = beats per min | offset - time between ticks
	void BPM::init(float value, int offset)
	{
		m_value = (float)value;
		m_offset = offset;
		initValues();

	}

	void BPM::initValues()
	{
		m_framecount = 0;
		m_previousbeatdist = 0;
		m_beat = 0.0f;

		m_changed = false;
		m_lastBeats = 0;
	}

	void BPM::update()
	{
		int aika = g_system->getTime();

		//TODO: offset support
		float minutes = aika / 60000.0f;
		int howmanybeats = (int)(m_value * minutes);

		if (howmanybeats > m_lastBeats)
		{
			m_changed = true;
			m_lastBeats = howmanybeats;
		}

		m_beat = 1.0f - (m_value * minutes - howmanybeats); //fractional part
		m_framecount = howmanybeats;
	}

	void BPM::clearFlags()
	{
		m_changed = false;
	}

	float BPM::getValue()
	{
		return m_beat;
	}

	int BPM::getBeatcount()
	{
		return m_framecount;
	}

	bool BPM::getChanged()
	{
		return m_changed;
	}
}