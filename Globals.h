#pragma once

//fix visual studio "debug information truncated to 255 bytes"-whining
#pragma warning ( disable : 4786)

//#define GLEW_STATIC /* use static version and not the DLL */
#include <windows.h>
#include "glew/glew.h"
#include "glfw/glfw3.h"
#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "core/DemoContext.h"

#include "util/Stringutils.h"
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
//TODO: remove 
#include "math/Range.h"
#include "math/Curve.h"
#include "math/LinearCurve.h"
#include "math/CatmullRom.h"
#include "math/Bezier.h"
#include "math/Mathematics.h"

#include "core/Debugger.h"
#include "core/Profiler.h"
#include "core/ErrorLog.h"
#include "core/Scene.h"	
#include "core/Demo.h"
#include "core/Sound.h"
#include "core/Parameter.h"
#include "core/Bpm.h"
#include "core/Trigger.h"
#include "core/Event.h"
#include "render/RenderDebug.h"
	#include "render/Buffer.h"
#include "render/Image.h"
#include "render/ImageFactory.h"
#include "render/TextureParameters.h"
#include "render/Texture.h"
#include "render/Cubemap.h"
#include "render/DepthMap.h"
#include "render/TextureManager.h"
#include "render/PostProcessManager.h"
#include "render/PostProcessEffect.h"
#include "render/RenderStatistics.h"
#include "render/Camera.h"
#include "render/OrthoCamera.h"
#include "render/Light.h"
#include "render/Mesh.h"
#include "render/Model.h"
#include "render/MeshManager.h"
#include "render/RenderTargetManager.h"
#include "render/Shader.h"
#include "render/ShaderManager.h"
#include "render/RenderUtils.h"
#include "render/Font.h"
#include "render/ShadowMap.h"
#include "core/SyncManager.h"
#include "core/Timer.h"
#include "core/System.h"

extern democore::Debugger g_debug;
extern democore::Profiler g_profiler;
extern democore::ErrorLog g_error;
extern democore::ErrorLog g_screenText;
extern const std::string g_prodName;