#pragma once

#include "../Globals.h"

namespace demorender
{

	class DepthMap
	{
	public:
		DepthMap();
		~DepthMap();

		bool create(const TextureParameters& params);
		void bind();
		void unbind();
		bool hasBeenUsed();

		GLuint getID() { return m_ID; };
		int getWidth();
		int getHeight();

	private:
		TextureParameters m_params;
		GLuint m_ID;
		GLuint m_FBO;
		bool m_hasBeenBound;
	};

}