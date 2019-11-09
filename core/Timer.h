#pragma once

#include "../Globals.h"

namespace democore
{

	class Timer
	{
	public:
		Timer();
		~Timer();

		void init();
		void start();
		void stop();
		void setPaused(bool paused);
		void addTime(int add);
		void setTime(int time);
		void update(int audioTime, int songLength); //kludge...
		int getTime();

	private:
		bool m_paused;
		int m_initialTime;
		int m_previousTime;
		int m_elapsedTime;

	};
}