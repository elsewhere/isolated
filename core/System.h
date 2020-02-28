#pragma once

#include "../Globals.h"
#include "Config.h"
#include "../render/LoadingScreen.h"

#define g_system System::inst()

namespace demorender
{
	class Mesh;
	class MeshInfo;
	class LoadingScreen;
}

namespace democore
{
	class Demo;
	class BPM;
	class TriggerSystem;
	class Event;
	class Image;
	class Texture;
	class TextureParameters;
	class Timer;

	class System
	{
	public:
		static System *inst();
		//initializing
		void init();
		void clear(); //clear out parameters & such
		void clearImages();
		void kill(); //free stuff
		void setDeveloperMode(bool dev) { m_developerMode = dev; };
		bool getDeveloperMode() { return m_developerMode; }

		bool getAltMode() { return m_altMode; }

		//window handling and system
		bool initOpenGL(Config& cfg);
//		GLWindow& getGLWindow();
//		GLSystem& getGLSystem();

		bool getKeyDown(int i);
		bool getKeyPressed(int i);
		void setWindowTitle(const std::string& title);
		void swapBuffers();
		void resetViewport();
		bool demoRunning();

		void updateFPS();
		float getFPS();

		//projection
		float getAspectRatio();

		//loading screen
		void addLoadingScreenSteps(int count);
		void advanceLoadingScreen(int count);
		void drawLoadingScreen();

		//update demo
		void update();

		//timer
		int getTime();
		void addTime(int adjust);
		void goToJumpTime();


		//sounds
		void setSoundEnabled(bool enabled);
		bool getSoundEnabled();
		void setSongFilename(const std::string& filename);
		void setEndTime(int time);
		int getEndTime();
		void setJumpTime(int time);
		int getJumpTime();
		std::string getSongFilename();
		bool initSound(const std::string& filename);
		void freeSound();
		void startSong();
		void stopSong();
		void toggleSound();
		void toggleDebug();
		void pauseSong(bool pause);
		void getWaveData(float *array, int size);
		void getSpectrum(float *array, int size);
		float getSpectrumSum(int start = 0, int end = 512);

		//debug
		bool isDebug();
		int getAudioPosition();

		void setRunning(bool running);
		bool exitRequested();

		GLFWwindow *getWindow();
		void getWindowSize(int &width, int &height);

	private:
		System();
		~System();

		GLFWwindow *m_pWindow;
		bool m_developerMode;
		bool m_running;
		bool m_debug;

		//loading screen
		int m_loadingScreenSteps;
		int m_loadingScreenPosition;

		//FPS
		int m_frameCount;
		int m_framePreviousTime;
		int m_frameTimer;
		float m_FPS;

		//GL stuff
		bool createWindow(Config &cfg);
		bool createSystem(Config &cfg);

		int m_endTime;
		int m_jumpTime;

		//sound
		Sound *m_audio;
		Song *m_song;
		std::string m_songFile;
		bool m_soundEnabled;
		bool m_dialogSoundEnabled;
		bool m_randomized;
		bool m_soundOn;
		bool m_altMode;

		//timer
		std::unique_ptr<Timer> m_timer;

		std::unique_ptr<demorender::LoadingScreen> m_loadingScreen;
		static System* sm_instance;
	};
}