#pragma once

#include "../Globals.h"

namespace demorender
{

	class Cubemap
	{
	public:
		enum Side
		{
			POS_X = 0,
			NEG_X = 1,
			POS_Y = 2,
			NEG_Y = 3,
			POS_Z = 4,
			NEG_Z = 5
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