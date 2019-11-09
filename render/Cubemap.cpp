#include "Cubemap.h"


namespace demorender
{
	Cubemap::Cubemap():m_ID(-1)
		, m_hasBeenBound(false)
	{
		m_params.resetDefaults();
	}

	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Cubemap::bind(int texunit)
	{
		glActiveTexture(texunit);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		m_hasBeenBound = true;
	}

	bool Cubemap::hasBeenUsed()
	{
		return m_params.m_upload && m_hasBeenBound;

	}

	bool Cubemap::create(const TextureParameters& params, std::vector<Image*>& images)
	{
		m_params = params;
		bool createEmpty = false;
		if (images.empty())
		{
			createEmpty = true;
			if (m_params.m_width <= 0 || m_params.m_height <= 0)
			{
				g_debug << "invalid dimensions for empty cubemap, defaulting to 1024^2!" << std::endl;
				m_params.m_width = 1024;
				m_params.m_height = 1024;
			}
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		if (createEmpty)
		{
			for (int i = 0; i < 6; i++)
			{
				if (m_params.m_format == TextureParameters::RGBA)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 4, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); GL_DEBUG;
				}
				else if (m_params.m_format == TextureParameters::FLOAT16)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, 0); GL_DEBUG;
				}
				else if (m_params.m_format == TextureParameters::FLOAT32)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, 0); GL_DEBUG;
				}
			}
		}
		else
		{
			m_params.m_width = images.at(0)->getWidth();
			m_params.m_height = images.at(0)->getHeight();
			//order is posX, negX, posY, negY, posZ, negZ
			for (int i = 0; i < 6; i++)
			{
				unsigned int *data = images.at(i)->getData();
				if (m_params.m_format == TextureParameters::RGBA)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 4, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); GL_DEBUG;
				}
				else if (m_params.m_format == TextureParameters::FLOAT16)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, data); GL_DEBUG;
				}
				else if (m_params.m_format == TextureParameters::FLOAT32)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, m_params.m_width, m_params.m_height, 0, GL_RGBA, GL_FLOAT, data); GL_DEBUG;
				}
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_params.m_linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_params.m_linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return true;
	}

	int Cubemap::getWidth()
	{
		return m_params.m_width;
	}
	int Cubemap::getHeight()
	{
		return m_params.m_height;
	}
}