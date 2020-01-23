#include "TextureParameters.h"
#include <sstream>

namespace demorender
{
	TextureParameters::TextureParameters()
	{
		resetDefaults();
	}
	TextureParameters::~TextureParameters()
	{
	}

	void TextureParameters::resetDefaults()
	{
		m_width = 0;
		m_height = 0;
		m_upload = true;
		m_retain = false;
		m_linear = true;
		m_repeat = true;
		m_mipmap = true;
		m_format = RGBA;
		m_type = TEXTURE_2D;
		m_internalFormat = 0;
		m_filename = "invalid";
		m_filenamePosX = "invalid";
		m_filenameNegX = "invalid";
		m_filenamePosY = "invalid";
		m_filenameNegY = "invalid";
		m_filenamePosZ = "invalid";
		m_filenameNegZ = "invalid";
	}

	std::string TextureParameters::toString()
	{
		std::stringstream ss;

		ss << "\n\nTextureParameters:" << "\n";
		ss << "filename = " << m_filename << "\n";
		ss << "size = " << m_width << "x" << m_height << "\n";
		ss << "upload = " << (m_upload ? "true" : "false") << "\n";
		ss << "retain = " << (m_retain ? "true" : "false") << "\n";
		ss << "linear = " << (m_linear ? "true" : "false") << "\n";
		ss << "mipmap = " << (m_mipmap ? "true" : "false") << "\n";
		ss << "format = ";
		if (m_format == RGBA)
			ss << "RGBA" << "\n";
		else if (m_format == FLOAT16)
			ss << "FLOAT16" << "\n";
		else if (m_format == FLOAT32)
			ss << "FLOAT32" << "\n";
		ss << "type = ";
		if (m_type == TEXTURE_2D)
			ss << "TEXTURE_2D" << "\n";
		else if (m_type == CUBEMAP)
			ss << "CUBEMAP" << "\n";
		else if (m_type == DEPTH)
			ss << "DEPTH" << "\n";

		return ss.str();
	}
}