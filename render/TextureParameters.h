#pragma once

#include <string>

namespace demorender
{
	class TextureParameters
	{
	public:
		enum Format
		{
			RGBA,
			FLOAT16,
			FLOAT32
		};
		enum Type
		{
			TEXTURE_2D,
			CUBEMAP,
			DEPTH,
			DEPTH_CUBEMAP,
		};
		TextureParameters();
		~TextureParameters();

		void resetDefaults();

		std::string toString();

		int m_width = 0;
		int m_height = 0;
		bool m_upload = true;
		bool m_retain = false;
		bool m_linear = true;
		bool m_repeat = true;
		bool m_mipmap = true;
		Format m_format = RGBA;
		unsigned int m_internalFormat = 0;
		Type m_type = TEXTURE_2D;

		std::string m_filename = "invalid";
		std::string m_filenamePosX = "invalid";
		std::string m_filenameNegX = "invalid";
		std::string m_filenamePosY = "invalid";
		std::string m_filenameNegY = "invalid";
		std::string m_filenamePosZ = "invalid";
		std::string m_filenameNegZ = "invalid";

	private:
	};
}