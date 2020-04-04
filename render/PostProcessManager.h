#pragma once

#include <string>
#include <deque>
#include <map>
#include <utility>
#include <variant>

#define g_postProcess PostProcessManager::inst()

namespace demorender
{
	class Camera;

	using Parameter = std::variant<int, float>;
	using PostProcessParameters = std::map<std::string, Parameter>;
	using StackItem = std::pair<class PostProcessEffect*, PostProcessParameters>;

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
		void addOldschoolRadialBlur();
		void addRadialGlow(int iterations = 10, float spread = 0.007f, float exponent = 1.f, float addAlpha = 1.0f);

//		void addFeedback();

		void addEndOfTheWorld(float power);

		void addLens(float focus, demorender::Camera* pCamera);
		void addSSAO(float nearPlane, float farPlane); //TODO

	private:

		PostProcessManager();
		~PostProcessManager();

		static PostProcessManager* sm_instance;

		std::deque<StackItem> m_stack;
		std::map<std::string, class PostProcessEffect*> m_effects;
	};
}