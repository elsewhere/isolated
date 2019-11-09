#include <algorithm>
#include "Demo.h"
#include "ScriptLoader.h"

namespace democore
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                          TimelineEntry
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	TimelineEntry::TimelineEntry()
	{
		m_endTime = 0;
		m_startTime = 0;
		m_scene = 0;
		m_priority = 0;
		m_name = "";
		m_frametimer = nullptr;
	}
	TimelineEntry::~TimelineEntry()
	{
		destroy();
	}

	void TimelineEntry::destroy()
	{
		if (m_frametimer)
		{
			delete m_frametimer;
			m_frametimer = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                Demo
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Demo::initOk()
	{
		return m_start;
	}
	Demo::Demo(DemoContextType type, const std::string& scriptfile):
	DemoContext(type),
	m_scriptName(scriptfile),
	m_endTime(0),
	m_running(false),
	m_exitRequested(false)
	{
		g_debug << "initializing demo" << std::endl;

		//parse script
		ScriptLoader loader;
		loader.load(scriptfile, this);

		//initialize configuration dialog
		m_cfg = new Config();
		m_cfg->run();

		if (m_cfg->getRunFlag())
		{
			//GLFW TODO
			g_system->initOpenGL(*m_cfg);

			std::string songPath = "data\\audio\\";
			std::string songFile = g_system->getSongFilename();
			g_debug << "loading sound " << songFile << std::endl;
			m_start = true;

			const int SOUND_LOADING_STEPS = 30; //put a lot in it since it takes long
			const int SOUND_PRESTEP = 2;
			g_system->addLoadingScreenSteps(SOUND_LOADING_STEPS);
			g_system->advanceLoadingScreen(SOUND_PRESTEP);

			//draw almost empty loading screen
			g_system->drawLoadingScreen();

			g_system->initSound(songPath + songFile);
			g_system->advanceLoadingScreen(SOUND_LOADING_STEPS-SOUND_PRESTEP);
		}
		else
		{
			m_start = false;
		}

	}

	Demo::~Demo()
	{
		g_debug << "demo quitting.." << std::endl;
		destroy();
		g_system->kill();
	}
	void Demo::setEndTime(int time)
	{
		m_endTime = time;
	}
	int Demo::getEndTime()
	{
		return m_endTime;
	}

	bool Demo::isRunning()
	{
		bool escape = g_system->getKeyPressed(GLFW_KEY_ESCAPE);
		bool demodone = (g_system->getTime() >= m_endTime);

		return !escape && !demodone;
	}

	bool Demo::isExitRequested()
	{
		return m_exitRequested;
	}

	void Demo::start()
	{
		g_debug << "starting demo now!" << std::endl;
		m_running = true;
		g_system->startSong();
	}

	void Demo::stop()
	{
		g_system->stopSong();
		g_debug << "demo stopped!" << std::endl;
	}

	void Demo::destroy()
	{
		if (m_start)
		{
			//clear out all the scenes in the demo
			for (auto& s : m_scenes)
			{
				delete s.second;
			}
			//and the timeline
			for (auto& e : m_timeline)
			{
				e->destroy();
			}
			m_timeline.clear();
			m_scenes.clear();

			//kill audio
			g_system->freeSound();
		}
	}

	void Demo::addScene(const std::string& name, Scene *s)
	{
		//insert the scene to the map
		m_scenes[name] = s;
		g_debug << "added scene \"" << name << "\" to the demo" << std::endl;
	}

	Scene* Demo::getScene(const std::string& name)
	{
		if (m_scenes.find(name) != m_scenes.end())
		{
			return m_scenes[name];
		}
		else
		{
			g_debug << "error! Trying to fetch scene " << name << " but it does not exist\n";
			return 0;
		}
	}

	void Demo::clearTimeline()
	{
		m_timeline.clear();
	}

	void Demo::addSceneToTimeline(const std::string& name, int startTime, int endTime, int priority)
	{
		//check if such a scene exists
//		if (m_scenes.find(name) != m_scenes.end())
		{
			//create timeline entry for it
			TimelineEntry* entry = new TimelineEntry();

			entry->m_scene = 0; //m_scenes[name];
			entry->m_startTime = startTime;
			entry->m_endTime = endTime;
			entry->m_priority = priority;
			entry->m_name = name;
			const int FPS = 100;
			const int MAXITERATIONS = 30;
			entry->m_frametimer = new FrameTimer(1000/FPS, MAXITERATIONS); //
			g_debug << "created a timeline entry for scene \"" << name << "\" at [" << startTime << "," << endTime << "]" << std::endl;

			m_timeline.push_back(entry);
		}
//		else
		{
//			g_debug << "ERROR! no scene with name \"" << name << "\" exists!" << endl;
			//no such scene exists
		}
	}

	void Demo::init()
	{
		for (TimelineEntry *e : m_timeline)
		{
			e->m_scene = m_scenes[e->m_name];
		}

		//call init on all effects
		std::map<std::string, Scene *>::iterator it;

		for (it = m_scenes.begin(); it != m_scenes.end(); it++)
		{
			if (auto pScene = (*it).second)
			{
				pScene->init();
			}
			else
			{
				g_debug << "scene " << (*it).first << " == nullptr!" << std::endl;
			}

		}
	}

	void Demo::releaseEffects()
	{
		for (auto& scene : m_scenes)
		{
			scene.second->release();
		}
	}

	std::vector<TimelineEntry*> Demo::getCurrentEffects()
	{
		std::vector<TimelineEntry*> effects;
		int time = g_system->getTime();
		for (TimelineEntry *e : m_timeline)
		{
			if (time >= e->m_startTime && time < e->m_endTime)
			{
				effects.push_back(e);
			}
		}
		sort(effects.begin(), effects.end(), [](auto a,  auto b) { return a->m_priority < b->m_priority; });
		return effects;
	}
	std::vector<TimelineEntry*> Demo::getEffects()
	{
		std::vector<TimelineEntry*> effects;
		int time = g_system->getTime();
		for (TimelineEntry *e : m_timeline)
		{
			effects.push_back(e);
		}
		sort(effects.begin(), effects.end(), [](auto a, auto *b) { return a->m_startTime < b->m_startTime; });
		return effects;
	}

	void Demo::draw()
	{
		std::vector<TimelineEntry*> activeEffects;

		int time = g_system->getTime();

		for (auto e : m_timeline)
		{
			if (time >= e->m_startTime && time < e->m_endTime)
			{
				activeEffects.push_back(e);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //TODO: refactor away

		sort(activeEffects.begin(), activeEffects.end(), [](auto a, auto b) { return a->m_priority < b->m_priority; });
		for (auto e : activeEffects)
		{
			demorender::g_postProcess->startFrame();
 			e->m_scene->draw();
			demorender::g_postProcess->commit();
			if (g_system->isDebug())
			{
				e->m_scene->debug();
			}
		}

	}

	void Demo::toggleRunning()
	{
		if (m_running)
		{
			m_running = false;
			g_system->pauseSong(true);
		}
		else
		{
			m_running = true;
			g_system->pauseSong(false);
		}
	}

	void Demo::handleInput()
	{
		if (g_system->getKeyPressed(GLFW_KEY_ESCAPE))
		{
			m_exitRequested = true;
		}

		if (g_system->getDeveloperMode())
		{
			bool F1 = g_system->getKeyPressed(GLFW_KEY_F1); //reload params & shaders
			bool F2 = g_system->getKeyPressed(GLFW_KEY_F2); //reload params & shaders & reinit effects

			bool CTRL = GetAsyncKeyState(VK_CONTROL) != 0;
			bool J = g_system->getKeyPressed(GLFW_KEY_J);	//jump point
			bool S = g_system->getKeyPressed(GLFW_KEY_S);	//toggle sound

			bool reload = (F1 || F2 || J) && !CTRL;
			bool reinit = (F2 || J) && !CTRL;

			if (g_system->getKeyPressed(GLFW_KEY_SPACE))
			{
 				toggleRunning();
			}
			if (g_system->getKeyPressed(GLFW_KEY_TAB))
			{
				g_system->toggleDebug();
			}

			if (reload)
			{
				g_error.clear();
				clearTimeline();
				ScriptLoader loader;
				loader.load(m_scriptName, this);
				demorender::g_shaders->freeShaders();
				demorender::g_shaders->loadShaders();
				init();
			}
			if (reinit)
			{
				g_error.clear();
				releaseEffects();
				init();
			}

			if (S)
			{
				bool sound = g_system->getSoundEnabled();
				g_system->setSoundEnabled(!sound);
			}

			static bool returnDown = false;
			static int returnTime = 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (!returnDown)
				{
					//write down time if return was pressed
					returnDown = true;
					returnTime = g_system->getTime();
				}
			}
			else
			{
				if (returnDown)
				{
					//return was raised
					int returnLength = g_system->getTime() - returnTime;
					std::stringstream output;
					output << returnTime;
					output << " ";
					output << returnLength;
					g_debug + (output.str());//(returnTime + " " + returnLength);
					returnDown = false;
				}
			}
		}
	}
	void Demo::update()
	{
		handleInput();
		if (m_running)
		{
			int time = g_system->getTime();

			for (auto& e : m_timeline)
			{
				if (time >= e->m_startTime && time < e->m_endTime)
				{
					float t = Math::calcPos<int>(time, e->m_startTime, e->m_endTime);

					e->m_scene->setPosition(t);
					e->m_frametimer->update(time);
					while (e->m_frametimer->stepsLeft())
					{
						e->m_scene->update();
						e->m_frametimer->endStep();
					}
				}	

			}
			g_sync->clearFlags();
		}
	}
}