#include "Range.h"
#include "Mathematics.h"

namespace demomath
{

	Range::Range():
	m_minimum(0),
	m_maximum(0)
	{
	}
	Range::Range(float minimum, float maximum)
	{
		set(minimum, maximum);
	}
	Range::~Range()
	{
	}

	void Range::set(float minimum, float maximum)
	{
		m_minimum = minimum;
		m_maximum = maximum;
	}


	float Range::getValue(float t)
	{
		return m_minimum + (m_maximum - m_minimum) * t;
	}

	float Range::getPositionInside(float t)
	{
		if (t < m_minimum)
			return 0.f;
		if (t > m_maximum)
			return 1.f;

		return (t - m_minimum) / (m_maximum - m_minimum); 
	}

	float Range::lerp(float t)
	{
		if (t < 0.0f)
			return m_minimum;
		if (t > 1.0f)
			return m_maximum;
		return m_minimum + (m_maximum - m_minimum) * t;
	}

	Range Range::lerp(Range& other, float t)
	{
		return Range(m_minimum + (other.m_minimum - m_minimum) * t, 
					 m_maximum + (other.m_maximum - m_maximum) * t);
	}

	float Range::getRandomValue()
	{
		return m_minimum + (m_maximum - m_minimum) * Math::randFloat();
	}
}