#pragma once

#include "../Globals.h"

namespace demorender
{

	class ImageFactory
	{
	public:
		static class Image* loadPNG(const std::string& filename);
		static class Image* loadJPG(const std::string& filename);
		static class Image* createEmpty(int width, int height);
	};
}
