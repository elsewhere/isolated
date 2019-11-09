#pragma once

#include "../Globals.h"

namespace demorender
{

	class Cubemap
	{
	public:
		enum Side
		{
			POS_X,
			NEG_X,
			POS_Y,
			NEG_Y,
			POS_Z,
			NEG_Z
		};

		Cubemap();
		~Cubemap();

		bool create(const TextureParameters& params, std::vector<Image*>& images);
		void bind(int texunit = 0);
		bool hasBeenUsed();

		GLuint getID() { return m_ID; };
		int getWidth();
		int getHeight();

	private:
		TextureParameters m_params;
		GLuint m_ID;
		bool m_hasBeenBound;
	};

}