#include "DemoContext.h"

namespace democore
{
	DemoContext::DemoContext():
	m_type(CONTEXT_TYPE_NONE)
	{
	}
	DemoContext::~DemoContext()
	{
	}
	DemoContext::DemoContext(DemoContextType type)
	{
		m_type = type;
	}

	void DemoContext::handleInput()
	{
	}

}