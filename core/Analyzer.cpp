#include "Analyzer.h"

namespace democore
{
	Analyzer::Analyzer(int size, int frames):
		m_size(size)
	{
		m_data = new float[size];
	}
	Analyzer::~Analyzer()
	{
		delete[] m_data;
	}

	void Analyzer::update()
	{
		g_system->getSpectrum(m_data, m_size);
	}

	float Analyzer::getSum(Mode mode)
	{
		if (mode == Mode::CURRENT)
		{
			float sum = 0.f;
			for (int i = 0; i < m_size; i++)
				sum += m_data[i];

			return sum / m_size;
		}
		else if (mode == Mode::WEIGHTED)
		{
			float sum = 0.f;

			for (int i = 0; i < m_size; i++)
				sum += m_data[i];

			float currentSum = sum / m_size;

			//high tech for low pass
			float value = Math::lerp(m_previousSum, currentSum, 0.2f);
			m_previousSum = value;
			
			return value;

		}
		return 0.f;
	}
}