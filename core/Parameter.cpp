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
		m_floats.clear();
		m_ints.clear();
		m_vectors.clear();
		m_strings.clear();
		m_bools.clear();
		m_blendModes.clear();
		m_ranges.clear();
		
	}
	glm::vec3 ParameterSpace::getVector3(const std::string& name)
	{
		if (m_vectors.find(name) == m_vectors.end())
		{
			g_debug << "trying to get Vector3 \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return glm::vec3(0, 0, 0);
		}
		else
		{
			return m_vectors[name];
		}
	}

	float ParameterSpace::getFloat(const std::string& name)
	{
		if (m_floats.find(name) == m_floats.end())
		{
			g_debug << "trying to get float \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return 0.0f;
		}
		else
		{
			return m_floats[name];
		}
	}

	Range ParameterSpace::getRange(const std::string& name)
	{
		if (m_ranges.find(name) == m_ranges.end())
		{
			g_debug << "trying to get range \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return Range(0, 0);
		}
		else
		{
			return m_ranges[name];
		}
	}
	int ParameterSpace::getInt(const std::string& name)
	{
		if (m_ints.find(name) == m_ints.end())
		{
			g_debug << "trying to get int \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return 0;
		}
		else
		{
			return m_ints[name];
		}
	}
	GLenum ParameterSpace::getBlendMode(const std::string& name)
	{
		if (m_blendModes.find(name) == m_blendModes.end())
		{
			g_debug << "trying to get blendmode \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return 0;
		}
		else
		{
			return m_blendModes[name];
		}
	}

	std::string ParameterSpace::getString(const std::string& name)
	{
		if (m_strings.find(name) == m_strings.end())
		{
			g_debug << "trying to get string \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return "";
		}
		else
		{
			return m_strings[name];
		}
	}
	bool ParameterSpace::getBool(const std::string& name)
	{
		if (m_bools.find(name) == m_bools.end())
		{
			g_debug << "trying to get bool \"" << name << "\" from params though it doesn't exist!" << std::endl;
			return "";
		}
		else
		{
			return m_bools[name];
		}
	}


	void ParameterSpace::addFloat(const std::string& name, float value)
	{
		if (m_floats.find(name) != m_floats.end())
		{
			g_debug << "trying to add float \"" << name << "\" with value " << value << " into params though it already exists!" << std::endl;
		}
		else
		{
			m_floats[name] = value;
		}
	}
	void ParameterSpace::addInt(const std::string& name, int value)
	{
		if (m_ints.find(name) != m_ints.end())
		{
			g_debug << "trying to add int \"" << name << "\" with value " << value << " into params though it already exists!" << std::endl;
		}
		else
		{
			m_ints[name] = value;
		}
	}
	void ParameterSpace::addBlendMode(const std::string& name, GLenum value)
	{
		if (m_blendModes.find(name) != m_blendModes.end())
		{
			g_debug << "trying to add int \"" << name << "\" with value " << value << " into params though it already exists!" << std::endl;
		}
		else
		{
			m_blendModes[name] = value;
		}
	}
	void ParameterSpace::addVector3(const std::string& name, glm::vec3 &value)
	{
		if (m_vectors.find(name) != m_vectors.end())
		{
			g_debug << "trying to add vector3 \"" << name << "\" into params though it already exists!" << std::endl;
		}
		else
		{
			m_vectors[name] = value;
		}
	}
	void ParameterSpace::addRange(const std::string& name, Range &value)
	{
		if (m_ranges.find(name) != m_ranges.end())
		{
			g_debug << "trying to add range \"" << name << "\" into params though it already exists!" << std::endl;
		}
		else
		{
			m_ranges[name] = value;
		}
	}

	void ParameterSpace::addString(const std::string& name, const std::string &value)
	{
		if (m_strings.find(name) != m_strings.end())
		{
			g_debug << "trying to add string  \"" << name << "\" into params though it already exists!" << std::endl;
		}
		else
		{
			m_strings[name] = value;
		}
	}
	void ParameterSpace::addBool(const std::string& name, bool value)
	{
		if (m_bools.find(name) != m_bools.end())
		{
			g_debug << "trying to add bool\"" << name << "\" into params though it already exists!" << std::endl;
		}
		else
		{
			m_bools[name] = value;
		}
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

	std::map<std::string, ParameterSpace>* Parameters::getAllParameters()
	{
		return &m_params;

	}

	//utils
	void Parameters::clear()
	{
		for (auto& m : m_params)
		{
			m.second.clear();
		}
	}

	void Parameters::save(const std::string& filename)
	{
	}

	void Parameters::useNamespace(const std::string& space)
	{
		//force lowercase
		std::string name = space;
		transform(name.begin(), name.end(), name.begin(), tolower);
		m_currentName = name;
	}

	//insertion
	void Parameters::addFloat(const std::string& space, const std::string& name, float value)
	{
		ParameterSpace &s = m_params[space];
		s.addFloat(name, value);
	}
	void Parameters::addBool(const std::string& space, const std::string& name, bool value)
	{
		ParameterSpace &s = m_params[space];
		s.addBool(name, value);
	}
	void Parameters::addBlendMode(const std::string& space, const std::string& name, GLenum value)
	{
		ParameterSpace &s = m_params[space];
		s.addBlendMode(name, value);
	}
	void Parameters::addInt(const std::string& space, const std::string& name, int value)
	{
		ParameterSpace &s = m_params[space];
		s.addInt(name, value);
	}
	void Parameters::addRange(const std::string& space, const std::string& name, Range &value)
	{
		ParameterSpace &s = m_params[space];
		s.addRange(name, value);
	}
	void Parameters::addVector3(const std::string& space, const std::string& name, glm::vec3 &value)
	{
		ParameterSpace &s = m_params[space];
		s.addVector3(name, value);
	}
	void Parameters::addString(const std::string& space, const std::string& name, std::string &value)
	{
		ParameterSpace &s = m_params[space];
		s.addString(name, value);
	}

	//get float
	float Parameters::getFloat(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getFloat(name);
	}
	float Parameters::getFloat(const std::string& name)
	{
		return getFloat(m_currentName, name);
	}

	//get int
	int Parameters::getInt(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getInt(name);
	}
	int Parameters::getInt(const std::string& name)
	{
		return getInt(m_currentName, name);
	}

	//get getblendmode
	GLenum Parameters::getBlendMode(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getBlendMode(name);
	}
	GLenum Parameters::getBlendMode(const std::string& name)
	{
		return getBlendMode(m_currentName, name);
	}

	//get bool
	bool Parameters::getBool(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getBool(name);
	}
	bool Parameters::getBool(const std::string& name)
	{
		return getBool(m_currentName, name);
	}

	//get vector3
	glm::vec3 Parameters::getVector3(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getVector3(name);
	}
	glm::vec3 Parameters::getVector3(const std::string& name)
	{
		return getVector3(m_currentName, name);
	}

	//get range
	Range Parameters::getRange(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getRange(name);
	}
	Range Parameters::getRange(const std::string& name)
	{
		return getRange(m_currentName, name);
	}

	std::string Parameters::getString(const std::string& space, const std::string& name)
	{
		ParameterSpace &s = m_params[space];
		return s.getString(name);
	}
	std::string Parameters::getString(const std::string& name)
	{
		return getString(m_currentName, name);
	}
}