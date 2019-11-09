#pragma once
#include <string>
#include <vector>
#include <map>

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
		ParameterSpace();
		~ParameterSpace();

		void setName(const std::string& name) { m_name = name; }
		std::string getName() { return m_name; }
		void clear();

		//insertion methods
		void addFloat(const std::string& name, float value);
		void addInt(const std::string& name, int value);
		void addVector3(const std::string& name, glm::vec3& value);
		void addString(const std::string& name, const std::string& value);
		void addBool(const std::string& name, bool value);
		void addBlendMode(const std::string& name, GLenum blendmode);
		void addRange(const std::string& name, demomath::Range& range);
		
		//accessors
		float getFloat(const std::string& name);
		int getInt(const std::string& name);
		glm::vec3 getVector3(const std::string& name);
		demomath::Range getRange(const std::string& name);
		std::string getString(const std::string& name);
		bool getBool(const std::string& name);
		GLenum getBlendMode(const std::string& name);

		std::map<std::string, float>* getAllFloats() { return &m_floats; }
		std::map<std::string, int>* getAllInts() { return &m_ints; }
		std::map<std::string, glm::vec3>* getAllVector3s() { return &m_vectors; }
		std::map<std::string, std::string>* getAllStrings() { return &m_strings; }
		std::map<std::string, bool>* getAllBools() { return &m_bools; }
		std::map<std::string, GLenum>* getAllBlendModes() { return &m_blendModes; }
		std::map<std::string, demomath::Range>* getAllRanges() { return &m_ranges; }


	private:
		std::string m_name;
		//these maps contain the parameters, one map per type. 
		std::map<std::string, float> m_floats;
		std::map<std::string, int> m_ints;
		std::map<std::string, glm::vec3> m_vectors;
		std::map<std::string, std::string> m_strings;
		std::map<std::string, bool> m_bools;
		std::map<std::string, GLenum> m_blendModes;
		std::map<std::string, demomath::Range> m_ranges;
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
		//write it into a file
		void save(const std::string& filename);

		std::map<std::string, ParameterSpace>* getAllParameters(); //probably should return iterators or something... 

		//insertion
		void addFloat(const std::string& space, const std::string& name, float value);
		void addInt(const std::string& space, const std::string& name, int value);
		void addVector3(const std::string& space, const std::string& name, glm::vec3 &value);
		void addString(const std::string& space, const std::string& name, std::string& value);
		void addBool(const std::string& space, const std::string& name, bool value);
		void addBlendMode(const std::string& space, const std::string& name, GLenum blendmode);
		void addRange(const std::string& space, const std::string& name, demomath::Range &range);

		//accessors
		float getFloat(const std::string& space, const std::string& name);
		float getFloat(const std::string& name);
		int getInt(const std::string& space, const std::string& name);
		int getInt(const std::string& name);
		std::string getString(const std::string& space, const std::string& name);
		std::string getString(const std::string& name);
		demomath::Range getRange(const std::string& space, const std::string& name);
		demomath::Range getRange(const std::string& name);
		glm::vec3 getVector3(const std::string& space, const std::string& name);
		glm::vec3 getVector3(const std::string& name);
		bool getBool(const std::string& space, const std::string& name);
		bool getBool(const std::string& name);
		GLenum getBlendMode(const std::string& space, const std::string& name);
		GLenum getBlendMode(const std::string& name);

	private:
		static Parameters* sm_instance;
		std::string m_currentName;
		std::map<std::string, ParameterSpace> m_params;


	};


}
