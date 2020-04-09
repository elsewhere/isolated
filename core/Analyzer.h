#pragma once

#include "../Globals.h"

namespace democore
{
	class Analyzer
	{
	public:
		enum class Mode
		{
			CURRENT,
			WEIGHTED,

			
		};

		Analyzer(int size, int frames);
		~Analyzer();

		void update();

		float getSum(Mode mode);

	private:
		float *m_data = nullptr;
		int m_size = 512;

		float m_previousSum = 0.f;

	};
}


