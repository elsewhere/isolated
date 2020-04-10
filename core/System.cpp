#include <tchar.h>
#include <algorithm>
#include "system.h"

#pragma warning(disable : 4204)

using namespace demorender;
using namespace demomath;

namespace democore
{
	void glfw_error_callback(int error, const char* description)
	{
		g_debug << "glfw error callback : " << error << " : " << description << std::endl;
	}
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			g_system->setRunning(false);
		}
		//not glfw but fuck that
		if (g_system->getDeveloperMode())
		{
			bool slowdown = GetAsyncKeyState(VK_RSHIFT) || GetAsyncKeyState(VK_LSHIFT);
			const int adjust = slowdown ? 2 : 1000;

			if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
				g_system->addTime(-adjust);
			if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
				g_system->addTime(adjust);
			if (key == GLFW_KEY_J && action == GLFW_PRESS)
				g_system->goToJumpTime();
			if (key == GLFW_KEY_S && action == GLFW_PRESS)
				g_system->toggleSound();
			if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
				g_system->toggleDebug();
			if (key == GLFW_KEY_SPACE)
			{

			}
		}
	}

	System* System::sm_instance = nullptr;

	System::System()
	{
		m_song = nullptr;
		m_audio = nullptr;
		m_loadingScreenSteps = 0;
		m_loadingScreenPosition = 0;
		m_soundEnabled = true;
		m_dialogSoundEnabled = true;
		m_developerMode = true;
		m_pWindow = nullptr;
		m_debug = false; 
		m_running = false;
	}
	System::~System()
	{
		if (m_song)
		{
			delete m_song;
		}
		if (m_audio)
		{
			delete m_audio;
		}

	}
	System* g_system
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new System();
		}
		return sm_instance;
	}


	void System::init()
	{
		m_loadingScreenSteps = 0;
		m_loadingScreenPosition = 0;

		m_frameCount = 0;
		m_frameTimer = 0;
		m_framePreviousTime = 0;
		m_FPS = 0.0f;
		m_soundOn = true;

		m_timer = std::make_unique<Timer>();
		setEndTime(1000);
	}

	void System::clear()
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                      Windowing and general stuff
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void System::kill()
	{
		g_debug << "terminating GLFW" << std::endl;
		glfwTerminate();
	}
	float System::getAspectRatio()
	{
		//GLFW TODO
		int width, height;
		glfwGetWindowSize(m_pWindow, &width, &height);
		return (float)width / (float)height;// m_glSystem->getScreenAspectRatio();
	}
	bool System::createWindow(Config &cfg)
	{
		int xres = cfg.getScreenX();
		int yres = cfg.getScreenY();
		int samples = 1 << cfg.getFsaa();
		if (samples == 1)
			samples = 0;

		//GLFW TODO
		/*
		if (!m_glWindow->createWindow(xres, yres, 32, cfg.getFullscreen(), cfg.getOnTop(), samples, cfg.getFrequency()))
		{
			g_debug << "ERROR! could not create window!\n" << endl;
			return false;
		}
		*/
		return true;
	}
	bool System::createSystem(Config &cfg)
	{
		//GLFW TODO
		/*
		if (!m_glSystem->init(cfg.getScreenX(), cfg.getScreenY()))
		{
			g_debug << "ERROR! could not initialize OpenGL!\n" << endl;
			return false;
		}
		*/
		m_dialogSoundEnabled = cfg.getSound();
		m_altMode = cfg.getAltMode();
		return true;
	}

	void System::setEndTime(int time)
	{
		m_endTime = time;
	}
	int System::getEndTime()
	{
		return m_endTime;
	}
	void System::setJumpTime(int time)
	{
		m_jumpTime = time;
	}
	int System::getJumpTime()
	{
		return m_jumpTime;
	}

	bool System::getKeyDown(int i)
	{
		//GLFW TODO
		return false;// return m_glWindow->getKeyDown(i);
	}
	bool System::getKeyPressed(int i)
	{
		int state = glfwGetKey(m_pWindow, i);
		return state == GLFW_PRESS;
	}

	bool System::initOpenGL(Config &cfg)
	{
		g_debug << "sound loaded, initializing GLFW" << std::endl;
		if (!glfwInit())
		{
			g_debug << "could not initialize GLFW!" << std::endl;
			return false;
		}
		glfwSetErrorCallback(glfw_error_callback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.1
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		m_pWindow = glfwCreateWindow(cfg.getScreenX(),
									 cfg.getScreenY(), 
									 "",
									 cfg.getFullscreen() ? glfwGetPrimaryMonitor() : NULL, 
									 NULL);
		if (!m_pWindow)
		{
			glfwTerminate();
			g_debug << "could not create GLFW window!" << std::endl;
		}
		glfwMakeContextCurrent(m_pWindow);
		glfwSetKeyCallback(m_pWindow, glfw_key_callback);
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		g_debug << "GLFW started, starting up GLEW" << std::endl;
		//init GLEW
		glewExperimental = GL_TRUE; //stops glew from crashing, thanks a lot GLEW
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			g_debug << "cannot initialize GLEW!" << std::endl;
		}
		if (!GLEW_VERSION_4_1)
		{
			g_debug << "OpenGL 4.1 is not available!" << std::endl;
		}
		else
		{
			g_debug << "found OpenGL 4.1!" << std::endl;
		}
		GL_DEBUG

		if (GLEW_KHR_debug)
		{
			g_debug << "We have OpenGL debugging extension" << std::endl;
			RenderDebug::inst()->setDebugging(true);
		}
		else
		{
			g_debug << "OpenGL debugging not available" << std::endl;
			RenderDebug::inst()->setDebugging(false);
		}

		m_loadingScreen = std::make_unique<demorender::LoadingScreen>();

		setRunning(true);
		return true;
	}

	void System::setRunning(bool running)
	{
		m_running = running;
	}

	bool System::exitRequested()
	{
		return glfwWindowShouldClose(m_pWindow) != 0 || GetAsyncKeyState(VK_ESCAPE);// && m_running;
	}

	void System::setWindowTitle(const std::string& title)
	{
		//GLFW TODO
		//		m_glWindow->setWindowTitle(title);
	}
	void System::resetViewport()
	{
		//GLFW TODO
		//		glViewport(0, 0, m_glWindow->getWidth(), m_glWindow->getHeight());
	}
	void System::swapBuffers()
	{
		//GLFW TODO
		//		SwapBuffers(m_glWindow->getHDC());
		glfwSwapBuffers(m_pWindow);
	}

	void System::update()
	{
		m_timer->update(getAudioPosition(), m_song->getLength());
		updateFPS();
		glfwPollEvents();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										    FPS counter
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void System::updateFPS()
	{
		int currentTime = getTime();//SongPosition();
		int dt = currentTime - m_framePreviousTime;
		m_framePreviousTime = currentTime;

		if (dt < 0)
			dt = -dt;

		m_frameCount++;
		m_frameTimer += dt;

		//update per second
		if (m_frameTimer > 1000)
		{
			m_FPS = 1000.0f * (m_frameCount / (float)m_frameTimer);
			m_frameCount = 0;
			m_frameTimer = 0;
		}

	}
	float System::getFPS()
	{
		return m_FPS;
	}

	GLFWwindow* System::getWindow()
	{
		return m_pWindow;
	}

	void System::getWindowSize(int& width, int& height)
	{
		glfwGetWindowSize(m_pWindow, &width, &height);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										    Loading screen
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void System::addLoadingScreenSteps(int count)
	{
		m_loadingScreenSteps += count;
	}

	void System::advanceLoadingScreen(int steps)
	{
		m_loadingScreenPosition += steps;
	}

	float radiusmod(float a, float t)
	{
		return 0.7f + 0.3f * sinf(a * 5 - t * 9);
	}
	void System::drawLoadingScreen()
	{
		if (m_loadingScreenSteps > 0)
		{
			const float percentage = m_loadingScreenPosition / (float)m_loadingScreenSteps;
			m_loadingScreen->draw(percentage);
			glFlush();
			swapBuffers();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//												Audio
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void System::toggleSound()
	{
		m_soundOn = !m_soundOn;
		m_audio->setVolume(m_song, m_soundOn ? 1.f : 0.f);
	}

	void System::setSongFilename(const std::string& filename)
	{
		m_songFile = filename;
	}
	std::string System::getSongFilename()
	{
		return m_songFile;
	}

	void System::setSoundEnabled(bool enabled)
	{
		m_soundEnabled = enabled && m_dialogSoundEnabled;

		if (m_audio)
		{
			m_audio->setVolume(m_song, 1.0f); //actual toggle is done through volume settings
		}
	}
	bool System::getSoundEnabled()
	{
		return m_soundEnabled && m_dialogSoundEnabled;
	}
	int System::getAudioPosition()
	{
		return m_song->getPosition();
	}

	int System::getTime()
	{
		return m_timer->getTime();
	}

	bool System::initSound(const std::string& filename)
	{
		m_soundOn = true;
		m_audio = Sound::create();
		m_audio->init();
		m_song = new Song();
		m_song->loadStream((char*)filename.c_str());
		m_audio->setEnabled(m_soundEnabled && m_dialogSoundEnabled); //hmmh
		m_audio->setVolume(m_song, 1.0f); //turns off the sound if necessary
		return true;
	}

	void System::freeSound()
	{
		stopSong();
		m_song->release();
		m_audio->close();
		m_audio = 0;
		m_song = 0;
	}

	void System::startSong()
	{
		m_song->start();
		m_timer->init();
	}

	void System::stopSong()
	{
		m_song->stop();
		m_timer->stop();
	}
	void System::pauseSong(bool pause)
	{
		m_song->setPaused(pause);
		m_timer->setPaused(pause);
	}

	void System::addTime(int adjust)
	{
		m_song->changePosition(adjust);
		m_timer->addTime(adjust);
	}

	void System::goToJumpTime()
	{
		m_song->setPosition(m_jumpTime);
		m_timer->setTime(m_jumpTime);
	}

	void System::getWaveData(float *array, int size)
	{
		m_song->getWaveData(array, size);
		array[size-1] = 0; //linux fix
	}
	void System::getSpectrum(float *array, int size)
	{
		m_song->getSpectrum(array, size);
		array[size-1] = 0; //linux fix
	}

	float System::getSpectrumSum(int start, int end)
	{
		float fft[512];
		float sum = 0.0f;
		m_song->getSpectrum(fft, 512);

		end = std::min(end, 511); //linux fix
		for (int i = 0; i < end; i++)
		{
			sum += fft[i];
		}
		return sum;

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											  Utilities
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void System::toggleDebug()
	{
		m_debug = !m_debug;
	}
	bool System::isDebug()
	{
		return m_debug;
	}

}