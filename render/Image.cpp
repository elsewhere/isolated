#include "Image.h"

namespace demorender
{

	Image::Image()
	{
		clear();
	}

	Image::~Image()
	{
		releaseData();

	}

	void Image::clear()
	{
		m_width = 0;
		m_height = 0;
		m_data = 0;
	}
	void Image::releaseData()
	{
		if (m_data)
		{
			delete [] m_data;
			m_data = 0;
		}
	}
}