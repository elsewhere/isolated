#include "RenderStatistics.h"

#include "../Globals.h"

using namespace demomath;
using namespace std;
using namespace democore;

namespace demorender
{
	int RenderStatistics::sm_meshesRendered = 0;
	int RenderStatistics::sm_meshesCulled = 0;
	int RenderStatistics::sm_shadersBound = 0;
	int RenderStatistics::sm_texturesBound = 0;

	RenderStatistics::RenderStatistics()
	{
	}
	RenderStatistics::~RenderStatistics()
	{
	}

	void RenderStatistics::clear()
	{
		sm_meshesRendered = 0;
		sm_meshesCulled = 0;
		sm_shadersBound = 0;
		sm_texturesBound = 0;
	}

}