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

		int m_width;
		int m_height;
		bool m_upload;
		bool m_retain;
		bool m_linear;
		bool m_repeat;
		bool m_mipmap;
		Format m_format;
		unsigned int m_internalFormat;
		Type m_type;

		std::string m_filename;
		std::string m_filenamePosX;
		std::string m_filenameNegX;
		std::string m_filenamePosY;
		std::string m_filenameNegY;
		std::string m_filenamePosZ;
		std::string m_filenameNegZ;

	private:
	};
}