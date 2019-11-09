#pragma once

#include "../Globals.h"
#include "TextureParameters.h"

namespace demorender
{

	class Image;

	class Texture
	{
	public:
		Texture();
		~Texture();

		GLuint getID() { return m_ID; };
		bool create(const TextureParameters &params, demorender::Image *pImage);
		void bind(int texunit = 0);
		bool hasBeenUsed();

		int getWidth();
		int getHeight();

		GLuint getFormat();

	private:
		TextureParameters m_params;
		GLuint m_ID;
		bool m_hasBeenBound;

		TextureParameters::Format m_format;

	};

}