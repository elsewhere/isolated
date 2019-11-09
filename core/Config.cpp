//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Globals.h"
#include "../resources/resource.h"
#include "Config.h"


#pragma warning (disable : 4996)

//-------------------------------------------------------
//	Globals and static members
//-------------------------------------------------------

namespace democore
{
	const int DEV_RESOLUTION_X = 1280;
	const int DEV_RESOLUTION_Y = 768;

	const int DEFAULT_MIN_X = 640;
	const int DEFAULT_MIN_Y = 768;

	std::vector<DEVMODE> devModes;

	int Config::sm_frequency;
	int Config::sm_resolution;
	bool Config::sm_fullscreen;
	bool Config::sm_sound;
	bool Config::sm_runFlag;
	bool Config::sm_alwaysOnTop;
	bool Config::sm_altmode;
	int Config::sm_desktopHeight;
	int Config::sm_desktopWidth;
	int Config::sm_fsaa;

	//-------------------------------------------------------
	//	Constructors and Destructor
	//-------------------------------------------------------

	Config::Config()
	{
		sm_frequency = 60;
		sm_resolution = 0;
		sm_fullscreen = false;
		sm_sound = false;
		sm_runFlag = false;
		sm_alwaysOnTop = false;
		sm_desktopWidth = 0;
		sm_desktopHeight = 0;
		sm_altmode = false;
		sm_fsaa = 4;

		DEVMODE currentDevMode;
		currentDevMode.dmSize = sizeof(DEVMODE);
		currentDevMode.dmDriverExtra = 0;
		EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &currentDevMode, 0);

		sm_desktopWidth  = currentDevMode.dmPelsWidth;
		sm_desktopHeight = currentDevMode.dmPelsHeight;

		int  nModeExist;
		DEVMODE devMode;
		int lastW=0, lastH=0;
		for (int i=0; ;i++) 
		{
			ZeroMemory(&devMode, sizeof(DEVMODE));
			devMode.dmSize = sizeof(DEVMODE);
			nModeExist = EnumDisplaySettings(NULL, i, &devMode);


			if (nModeExist != 1) 
			{
			  break;
			}
			else if(devMode.dmBitsPerPel >= 32 
				&& devMode.dmPelsWidth >= DEFAULT_MIN_X
				&& devMode.dmPelsHeight >= DEFAULT_MIN_Y)
			{
				devModes.push_back(devMode);
			}
		}
	}
	Config::~Config()
	{
	}

	//-------------------------------------------------------
	//	Public functions
	//-------------------------------------------------------

	bool Config::run()
	{	
 		if(DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETUPDLG), NULL, (DLGPROC)ConfigProc) == 1)
		{		
			return false;
		}

		if(!getRunFlag()) 
			return false;

		return true;
	}

	int Config::getScreenX()
	{
		return devModes[sm_resolution].dmPelsWidth;
	}

	int Config::getScreenY()
	{
		return devModes[sm_resolution].dmPelsHeight;
	}

	bool Config::getFullscreen()
	{
		return sm_fullscreen;
	}
	bool Config::getAltMode()
	{
		return sm_altmode;
	}
	int Config::getFsaa()
	{
		return sm_fsaa;
	}

	bool Config::getSound()
	{
		return sm_sound;
	}

	int Config::getFrequency()
	{
		return devModes[sm_resolution].dmDisplayFrequency;
	}

	bool Config::getOnTop()
	{
		return sm_alwaysOnTop;
	}

	bool Config::getRunFlag()
	{
		return sm_runFlag;
	}

	//-------------------------------------------------------
	//	Dialog procedure - handles inputs
	//-------------------------------------------------------

	int Config::ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		switch(uMsg)
		{				
			case WM_INITDIALOG:
			{				
				InitCommonControls();
				
				RECT rect, dlgRect;
				HWND dlg;
				
				// Center the dialog box
				dlg = GetDesktopWindow();			
				if(dlg)
				{				
					GetWindowRect(dlg, &rect);
					GetWindowRect(hwnd, &dlgRect);
					SetWindowPos(hwnd, HWND_TOPMOST, (rect.right/2) - ((dlgRect.right - dlgRect.left) / 2), (rect.bottom/2) - ((dlgRect.bottom - dlgRect.top) / 2), 0, 0, SWP_NOSIZE);
					SetWindowText(hwnd, g_prodName.c_str());
				}			

				HWND hwndList;
				HWND hwndMultisample;

				// -- resolution
				hwndList = GetDlgItem(hwnd, IDC_RESOLUTION);
				hwndMultisample = GetDlgItem(hwnd, IDC_MULTISAMPLE);

				//iterate multisamples

				std::vector<int> samples;
				samples.push_back(0);
				samples.push_back(2);
				samples.push_back(4);
				samples.push_back(8);
				samples.push_back(16);
//				samples.push_back(16);

				int sampleIndex;
				for (int i = 0; i < (int)samples.size(); i++)
				{
					char buf[100];
					if (samples[i] == 0)
					{
						sprintf(buf, "no multisampling");
					}
					else
					{
						sprintf(buf, "%dx multisample", samples[i]);
					}
					sampleIndex = SendMessage(hwndMultisample, CB_ADDSTRING, i, (LPARAM)(LPCSTR)buf);
				}
				sm_fsaa = sampleIndex - 1;
				SendMessage(hwndMultisample, CB_SETCURSEL, sm_fsaa, 0);

				//iterate screen modes
		
				int defaultMode = 0;
				int i = 0;

				bool developermode = g_system->getDeveloperMode();
				for (auto& mode : devModes) 
				{
					char buf[100];
					sprintf(buf, "%dx%d 32 bpp %d hz", mode.dmPelsWidth, mode.dmPelsHeight, mode.dmDisplayFrequency);
					int z = SendMessage(hwndList, CB_ADDSTRING, i, (LPARAM)(LPCSTR)buf);
		
					if (!developermode)
					{
						if(mode.dmPelsWidth==sm_desktopWidth  && mode.dmPelsHeight==sm_desktopHeight)
							defaultMode = z;
					}
					else
					{
						if(mode.dmPelsWidth==DEV_RESOLUTION_X && mode.dmPelsHeight==DEV_RESOLUTION_Y)
						{
							defaultMode = z;
						}
					}
				}

							
				SendMessage(hwndList, CB_SETCURSEL, defaultMode, 0);
				sm_resolution = defaultMode;
			
				// Show the dialog
				ShowWindow(hwnd, SW_SHOW);
				SetActiveWindow(hwnd);
				SetForegroundWindow(hwnd);
				SetFocus(hwnd);

				// Set icon
				SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)));

				// Check default values
				if (!g_system->getDeveloperMode())
					SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_SETCHECK, wParam, 0);
				//SendDlgItemMessage(hwnd, IDC_RANDOMIZE, BM_SETCHECK, wParam, 0);
				SendDlgItemMessage(hwnd, IDC_SOUND, BM_SETCHECK, wParam, 0);

				// Set ONTOP to disabled or enabled at startup
				if(IsDlgButtonChecked(hwnd, IDC_FULLSCREEN)==BST_CHECKED)
				{
					EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), FALSE);
				}
				else 
				{
					EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), TRUE);
				}


			} break;

			case WM_COMMAND:
			{
				if(IsDlgButtonChecked(hwnd, IDC_FULLSCREEN)==BST_CHECKED)
					EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), FALSE); 
				 else 
					EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), TRUE); 
				
				
				switch(LOWORD(wParam))
				{
					
					case IDOK:
					{					
						sm_resolution = SendMessage(GetDlgItem(hwnd, IDC_RESOLUTION), CB_GETCURSEL, 0, 0);
						sm_fsaa = SendMessage(GetDlgItem(hwnd, IDC_MULTISAMPLE), CB_GETCURSEL, 0, 0);

						if(SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_GETSTATE, 0, 0) == BST_CHECKED) 
						{
							sm_fullscreen = true;
						} 

						if(SendDlgItemMessage(hwnd, IDC_SOUND, BM_GETSTATE, 0, 0) == BST_CHECKED) 
						{
							sm_sound = true;
						}
						else
						{
							sm_sound = false;
						}

						if(SendDlgItemMessage(hwnd, IDC_ONTOP, BM_GETSTATE, 0, 0) == BST_CHECKED) 
						{
							sm_alwaysOnTop = true;
						}
						if(SendDlgItemMessage(hwnd, IDC_ALTMODE, BM_GETSTATE, 0, 0) == BST_CHECKED) 
						{
							sm_altmode = true;
						}
						
						sm_runFlag = true;
						EndDialog(hwnd, 0);
					} break;
					
					case IDCANCEL:
					{
						sm_runFlag = false;
						SendMessage(hwnd, WM_CLOSE, 0, 0);
					} break;

				} break;

			} break;

			case WM_DESTROY:
			{
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			} break;
			
			case WM_CLOSE:
			{						
				EndDialog(hwnd, 0);
			} 
			break;		
		}

		

		return 0;
	}
}
