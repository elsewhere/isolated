#pragma once

#include <string>
#include <deque>
#include <map>
#include <utility>

#define g_postProcess PostProcessManager::inst()

namespace demorender
{
	class Camera;

	//TODO: this is pretty awful, should really use a template or something but.. 
	class Parameter
	{
	public:
		enum Type
		{
			FLOAT,
			INT
		};
		Parameter() :
			m_type(FLOAT),
			m_floatValue(0.f),
			m_intValue(0)
		{
		}
		//TODO: error checking
		float getFloat() { return m_floatValue; }
		int getInt() { return m_intValue; }

		void setFloat(float value) { m_type = FLOAT;  m_floatValue = value; };
		void setInt(int value) { m_type = INT;  m_intValue = value; };
		bool isFloat() { return m_type == FLOAT; }
		bool isInt() { return m_type == INT; }
	private:
		Type m_type;
		float m_floatValue;
		int m_intValue;
	};

	typedef std::map<std::string, Parameter> PostProcessParameters;

	class PostProcessManager
	{
	public:
		bool init();
		void startFrame();
		void commit();

		static PostProcessManager* inst();
		
		void addTest(); //for testing shit
		void addInvert();
		void addSobel(float spacing = 0.001f, float exponent = 1.0f);
		void addGlow(int iterations = 5, float spreadX = 0.001f, float spreadY = 0.001f, float exponent = 1.f, float addAlpha = 1.0f);
		void addDOF(float focus, float nearPlane, float farPlane);

		void addLens(float focus, demorender::Camera* pCamera);
		void addSSAO(float nearPlane, float farPlane); //TODO

	private:
		Parameter createInt(int value) { Parameter p; p.setInt(value); return p; }
		Parameter createFloat(float value) { Parameter p; p.setFloat(value); return p; }
		typedef std::pair<class PostProcessEffect*, PostProcessParameters> StackItem;

		PostProcessManager();
		~PostProcessManager();

		static PostProcessManager* sm_instance;

		std::deque<StackItem> m_stack;
		std::map<std::string, class PostProcessEffect*> m_effects;
	};
}