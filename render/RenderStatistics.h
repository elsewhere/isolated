#pragma once

#include "../Globals.h"

namespace demorender
{
	class RenderStatistics
	{
	public:
		RenderStatistics();
		~RenderStatistics();

		static void clear();

		static int sm_meshesRendered;
		static int sm_meshesCulled;
		static int sm_shadersBound;
		static int sm_texturesBound;

	private:
	};
}