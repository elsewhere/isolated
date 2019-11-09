#pragma once

//#include "../Globals.h"

namespace democore
{
	//unpleasant but necessary.. 
	enum DemoContextType
	{
		CONTEXT_TYPE_NONE = 0,
		CONTEXT_TYPE_DEMO = 1,
		CONTEXT_TYPE_MUSICDISK = 2, 
	};

	class DemoContext
	{
	public:
		DemoContext();
		DemoContext(DemoContextType type);
		~DemoContext();

		virtual void handleInput() = 0;
		virtual bool initOk() = 0;
		virtual bool isRunning() = 0;
		virtual void init() = 0;
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual bool isExitRequested() = 0;

		void setType(DemoContextType type) { m_type = type; }
		DemoContextType getType() { return m_type; }
	private:
		DemoContextType m_type;

	};
}