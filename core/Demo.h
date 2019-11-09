#pragma once

#include "DemoContext.h"
#include "../Globals.h"
#include "Sound.h"
#include "Config.h"
#include "FrameTimer.h"

/*
    An entry in the timeline. It has a pointer to the actual effect that it'll run, plus a bunch of other related stuff. 
*/

namespace democore
{
	class DemoContext;

	class TimelineEntry
	{
	public:
		TimelineEntry();
		~TimelineEntry();
		void destroy();

		Scene *m_scene; //ptr to the scene
		FrameTimer *m_frametimer; //frameskipper
		int m_startTime; //starts here
		int m_endTime;   //ends then
		int m_priority;  //priority in which the simultaneous effects will be drawn. bigger priority will be drawn first
		std::string m_name;   //name with what this is referenced
	};

	/*
		The main class for the demo. 
	*/


	class Demo : public DemoContext
	{
	public:
		Demo(DemoContextType type, const std::string& scriptfile); 
		~Demo();

		//returns true if we are all go for launch
		bool initOk();

		//returns true if the demo is running
		bool isRunning();

		//returns true if esc is pressed
		bool isExitRequested();

		//start the demo
		void start(); 
		//stop the demo
		void stop();

		//draw currently active scenes
		void draw();
		
		void handleInput();
		//update currently active scenes
		void update();
		//clear everything
		void destroy();

		//add a scene to the scene list
		void addScene(const std::string& name, Scene* s);
		//add a scene to the timeline
		void addSceneToTimeline(const std::string& name, int startTime, int endTime, int priority);
		//clear everything inside the timeline
		void clearTimeline();
		//get scene from the scene list
		Scene* getScene(const std::string& name);

		std::vector<TimelineEntry*> getCurrentEffects();
		std::vector<TimelineEntry*> getEffects();

		//inits all
		void init();
		//releases all effects
		void releaseEffects();
		//toggles running state
		void toggleRunning();

		void setEndTime(int time);
		int	getEndTime();

	private:
		int m_endTime;
		//this contains all the effects
		std::map<std::string, Scene*> m_scenes;
		//this contains all the timeline entries that will be played
		std::vector<TimelineEntry*> m_timeline;

		Config *m_cfg;
		bool m_start;
		std::string m_scriptName;
		bool m_running;
		bool m_exitRequested;
	};
}
