#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>

#include "../Globals.h"
#define g_params Parameters::inst()

namespace democore
{

	/*
		One single "parameter namespace" that contains parameters for one scene.
	*/

	class ParameterSpace
	{
	public:
		using Parameter = std::variant<float, int, glm::vec3, std::string, bool, demomath::Range>;

		ParameterSpace();
		~ParameterSpace();

		void setName(const std::string& name) { m_name = name; }
		std::string getName() { return m_name; }
		void clear();

		template<typename T>
		T get(const std::string& name)
		{
			return std::get<T>(m_values[name]);
		}

		void add(const std::string& name, Parameter param)
		{
			m_values[name] = param;
		}

	private:
		std::string m_name;
		std::map < std::string, Parameter> m_values;

	};

	/*
		A global object that contains all the parameters read from the script file. 
	*/

	class Parameters
	{
	public:
		Parameters();
		~Parameters();

		static Parameters *inst();
		//sets the current namespace so you can reference params only by their name
		void useNamespace(const std::string& space);
		//clear it all out
		void clear();

		template<typename T>
		void add(const std::string& name, T value)
		{
			Parameter param = T;
			m_params[m_currentName].add(name, param);
		}

		template<typename T>
		void add(const std::string& namespaceName, const std::string& name, T value)
		{
			ParameterSpace::Parameter param = value;
			m_params[namespaceName].add(name, param);
		}

		template<typename T>
		T get(const std::string& name)
		{
			return m_params[m_currentName].get<T>(name);
		}

		template<typename T>
		T get(const std::string& namespaceName, const std::string& name)
		{
			std::string temp = namespaceName;
			transform(temp.begin(), temp.end(), temp.begin(), tolower);
			return m_params[temp].get<T>(name);
		}

	private:
		static Parameters* sm_instance;
		std::string m_currentName;
		std::map<std::string, ParameterSpace> m_params;


	};


}
