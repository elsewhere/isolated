#include "PostProcessManager.h"
#include "../Globals.h" //why is this needed but not the others? 
#include "PostProcessEffect.h"

namespace
{
	const static bool EXTRA_DEBUG = false;
}

namespace demorender
{
	PostProcessManager* PostProcessManager::sm_instance = nullptr;
	
	PostProcessManager::PostProcessManager()
	{

	}
	
	PostProcessManager::~PostProcessManager()
	{

	}

	PostProcessManager* PostProcessManager::inst()
	{
		if (!sm_instance)
		{
			g_debug << "Creating PostProcessManager" << std::endl;
			sm_instance = new PostProcessManager();
			if (sm_instance->init())
			{
				g_debug << "PostrProcessManager created succesfully!" << std::endl;
			}
			else
			{
				g_debug << "Error in creating PostProcessManager!" << std::endl;
				delete sm_instance;
				sm_instance = nullptr;
			}
		}



		return sm_instance;
	}

	bool PostProcessManager::init()
	{
		m_effects["test"] = new TestEffect();
		m_effects["invert"] = new Invert();
		m_effects["sobel"] = new Sobel();
		m_effects["glow"] = new Glow();
		m_effects["ssao"] = new SSAO();
		m_effects["dof"] = new DOF();
		m_effects["lens"] = new Lens();
		m_effects["radial"] = new Radial();
		return true;
	}


	void PostProcessManager::addInvert()
	{
		StackItem item;
		item.first = m_effects["invert"];
		item.second.clear();
		m_stack.push_back(item);
	}

	void PostProcessManager::addTest()
	{
		StackItem item;
		item.first = m_effects["test"];
		item.second.clear();
		m_stack.push_back(item);
	}

	void PostProcessManager::addRadial()
	{
		StackItem item;
		item.first = m_effects["radial"];
		m_stack.push_back(item);
	}

	void PostProcessManager::addSobel(float spacing, float exponent)
	{
		StackItem item;
		item.first = m_effects["sobel"];
		item.second.clear();

		item.second["spacing"] = createFloat(spacing);
		item.second["exponent"] = createFloat(exponent);

		m_stack.push_back(item);
	}

	void PostProcessManager::addGlow(int iterations, float spreadX, float spreadY, float exponent, float addAlpha)
	{
		StackItem item;
		item.first = m_effects["glow"];
		item.second.clear();

		item.second["iterations"] = createInt(iterations);
		item.second["spreadX"] = createFloat(spreadX);
		item.second["spreadY"] = createFloat(spreadY);
		item.second["exponent"] = createFloat(exponent);
		item.second["alpha"] = createFloat(addAlpha);

		m_stack.push_back(item);
	}

	void PostProcessManager::addSSAO(float nearPlane, float farPlane)
	{
		StackItem item;
		item.first = m_effects["ssao"];
		item.second.clear();
		item.second["near"] = createFloat(nearPlane);
		item.second["far"] = createFloat(farPlane);

		m_stack.push_back(item);
	}

	void PostProcessManager::addDOF(float focus, float nearPlane, float farPlane)
	{
		StackItem item;
		item.first = m_effects["dof"];
		item.second.clear();
		item.second["focus"] = createFloat(focus);
		item.second["near"] = createFloat(nearPlane);
		item.second["far"] = createFloat(farPlane);

		m_stack.push_back(item);
	}

	void PostProcessManager::addLens(float focus, demorender::Camera* pCamera)
	{
		StackItem item;
		item.first = m_effects["lens"];
		item.second.clear();
		item.second["focus"] = createFloat(focus);
		item.second["near"] = createFloat(pCamera->getNearPlane());
		item.second["far"] = createFloat(pCamera->getFarPlane());

		m_stack.push_back(item);
	}

	void PostProcessManager::startFrame()
	{
		m_stack.clear();
	}

	void PostProcessManager::commit()
	{
		//push_back() ja pop_front()

		if (m_stack.empty())
		{
			//no postprocessing, just draw into backbuffer
			g_renderTargets->unbind();
			//blit to backbuffer
			g_renderUtils->fullscreenQuad("main");
			return;
		}
		else
		{
			if (EXTRA_DEBUG)
			{
				g_debug << "alussa stackissa " << m_stack.size() << " filtteriä" << std::endl;
			}
			//first one goes from main to post1
			std::string source = "main";
			std::string target = "post1";

			bool even = true;
			while (!m_stack.empty())
			{
				//do postprocessing pass 
				StackItem item = m_stack.front();
				PostProcessEffect* pEffect = item.first;
				if (EXTRA_DEBUG)
				{
					g_debug << "  apply effect " << pEffect->getName() << " from " << source << " to " << target << std::endl;
				}
				pEffect->apply(source, target, item.second);

				m_stack.pop_front();
				if (even)
				{
					even = false;
					source = "post1";
					target = "post2";
				}
				else
				{
					even = true;
					source = "post2";
					target = "post1";
				}
			}

			g_renderTargets->unbind();
			//blit to backbuffer
			if (EXTRA_DEBUG)
			{
				g_debug << "  blitting " << source << " to back buffer" << std::endl;
			}
			g_renderUtils->fullscreenQuad(source);
		}
		glDepthMask(1);
		glEnable(GL_DEPTH_TEST);
	}

}