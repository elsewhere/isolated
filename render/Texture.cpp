#include "Texture.h"

namespace
{
	static const bool EXTRA_DEBUG = false;
}

namespace demorender
{
	Texture::Texture():m_ID(-1)
		,m_hasBeenBound(false)
	{
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	bool Texture::hasBeenUsed()
	{
		return m_hasBeenBound && m_params.m_upload;
	}

	int Texture::getWidth()
	{
		return m_params.m_width;
	}
	int Texture::getHeight()
	{
		return m_params.m_height;
	}

	GLuint Texture::getFormat()
	{
		return m_params.m_internalFormat;
	}

	void Texture::bind(int texunit)
	{
		glActiveTexture(texunit);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		m_hasBeenBound = true;
	}

	bool Texture::create(const TextureParameters &params, Image *pImage)
	{
		m_params = params;

		bool isImage = pImage != 0;
		bool isRenderTarget = !isImage && m_params.m_width > 0 && m_params.m_height > 0;

		if (!isImage && !isRenderTarget)
		{
			g_debug << "size " << m_params.m_width << "x" << m_params.m_height << std::endl;
			g_debug << "cannot create texture! texture descriptor is invalid!" << std::endl;
			return false;
		}

		//create texture
		GLuint *pData = 0;

		if (isImage)
		{
			m_params.m_width = pImage->getWidth();
			m_params.m_height = pImage->getHeight();
			pData = pImage->getData();
			if (pData == 0)
			{
				g_debug << "Texture image contains no data!" << std::endl;
				return false;
			}
		}
		else
		{
			//width and height already set
			pData = 0;
		}

		if (EXTRA_DEBUG)
		{
			g_debug << "Creating texture!" << std::endl;
			g_debug << m_params.toString();
		}

		glGenTextures(1, &m_ID); GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, m_ID); GL_DEBUG;

		int filter = params.m_linear ? GL_LINEAR : GL_NEAREST;
		int wrap = params.m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		if (m_params.m_format == TextureParameters::RGBA)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData); GL_DEBUG;
			m_params.m_internalFormat = GL_RGBA8;
		}
		else if (m_params.m_format == TextureParameters::FLOAT16)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, pData); GL_DEBUG;
			m_params.m_internalFormat = GL_RGBA16F;
		}
		else if (m_params.m_format == TextureParameters::FLOAT32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, pData); GL_DEBUG;
			m_params.m_internalFormat = GL_RGBA32F;
		}


		if (m_params.m_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D); GL_DEBUG;
		}
		return true;
	}
}