#pragma warning ( disable : 4786)
#include "Parameter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                        ParameterSpace
///////////////////////////////////////////////////////////////////////////////////////////////////

using namespace demomath;
using namespace demorender;

namespace democore
{

	ParameterSpace::ParameterSpace()
	{
	}

	ParameterSpace::~ParameterSpace()
	{
	}

	void ParameterSpace::clear()
	{
		m_values.clear();
		
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//                                        Parameters
	///////////////////////////////////////////////////////////////////////////////////////////////////

	Parameters* Parameters::sm_instance = nullptr;

	Parameters::Parameters()
	{
		m_currentName = "";
	}

	Parameters::~Parameters()
	{
	}


	Parameters *g_params
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new Parameters();
		}
		return sm_instance;
	}

	//utils
	void Parameters::clear()
	{
		for (auto& m : m_params)
		{
			m.second.clear();
		}
	}

	void Parameters::useNamespace(const std::string& space)
	{
		//force lowercase
		std::string name = space;
		transform(name.begin(), name.end(), name.begin(), tolower);
		m_currentName = name;
	}
}