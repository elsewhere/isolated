#pragma once

#include "stdio.h"
#include <vector>
#include <windows.h>

/*
    Config class used for the demo settings. Includes the startup dialog.
*/
namespace democore
{

	class Config
	{		
		public:
			
			Config();
			~Config();

			bool run();
			
			// getterit
			int getScreenX();
			int getScreenY();
			int getFrequency();
			int getFsaa();
			bool getFullscreen();
			bool getSound();
			bool getOnTop();
			bool getAltMode();
			bool getRunFlag();
			
			static int ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			
			static int sm_resolution, sm_frequency;
			static bool sm_fullscreen, sm_sound, sm_runFlag, sm_alwaysOnTop;
			static int sm_desktopWidth, sm_desktopHeight;
			static bool sm_altmode;
			static int sm_fsaa;

		private: 

	};
}
