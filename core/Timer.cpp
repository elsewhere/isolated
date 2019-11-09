#include "Timer.h"
#include <mmsystem.h>

namespace democore
{

	Timer::Timer()
	{
		init();
	}

	Timer::~Timer()
	{
	}

	void Timer::init()
	{
		m_initialTime = timeGetTime();
		m_previousTime = m_initialTime;
		m_paused = false;
		m_elapsedTime = 0;
	}
	void Timer::start()
	{
	}
	void Timer::stop()
	{
	}
	void Timer::addTime(int add)
	{
		m_elapsedTime += add;
		if (m_elapsedTime < 0)
		{
			m_elapsedTime = 0;
		}
	}

	void Timer::setTime(int time)
	{
		m_elapsedTime = time;
		if (m_elapsedTime < 0)
			m_elapsedTime = 0;
		
	}
	void Timer::setPaused(bool paused)
	{
		m_paused = paused;
	}
	void Timer::update(int audioPosition, int songLength)
	{
		int currentTime = timeGetTime();
		int dt = currentTime - m_previousTime;
		m_previousTime = currentTime;

		if (m_paused)
		{
		}
		else
		{
			m_elapsedTime += dt;

			const int LIMIT = 30;
			if (abs(m_elapsedTime - audioPosition) > LIMIT && m_elapsedTime <= songLength-40)
			{
//				g_debug << "audio out of sync! timer elapsed time = " << m_elapsedTime << " audio timer = " << audioPosition << " delta = " << (audioPosition - m_elapsedTime) << "\n";
				m_elapsedTime = audioPosition;
			}
		}
	}

	int Timer::getTime()
	{
		return m_elapsedTime;
	}
}