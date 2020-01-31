#include "DepthMap.h"

namespace
{
	static const bool EXTRA_DEBUG = true;
}

namespace demorender

{
	DepthMap::DepthMap():m_ID(-1)
		, m_hasBeenBound(false)
	{
		m_params.resetDefaults();
	}

	DepthMap::~DepthMap()
	{
		glDeleteTextures(1, &m_ID);
	}

	void DepthMap::bind()
	{
		glViewport(0, 0, m_params.m_width, m_params.m_height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void DepthMap::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int width, height;
		//TODO: refactor this somehow
		democore::g_system->getWindowSize(width, height);
		glViewport(0, 0, width, height); 
	}
	bool DepthMap::hasBeenUsed()
	{
		return m_params.m_upload && m_hasBeenBound;
	}

	bool DepthMap::create(const TextureParameters& params)
	{
		m_params = params;

		if (EXTRA_DEBUG)
		{
			g_debug << m_params.toString() << std::endl;
		}

		glGenFramebuffers(1, &m_FBO); GL_DEBUG;
		glGenTextures(1, &m_ID); GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, m_ID); GL_DEBUG;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_params.m_width, m_params.m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); GL_DEBUG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GL_DEBUG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GL_DEBUG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); GL_DEBUG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); GL_DEBUG;

		float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); GL_DEBUG;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ID, 0); GL_DEBUG;
		glDrawBuffer(GL_NONE); GL_DEBUG;
		glReadBuffer(GL_NONE); GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_DEBUG;

		if (EXTRA_DEBUG)
		{
			g_debug << "depth map FBO = " << m_FBO << " ID = " << m_ID << std::endl;
		}

		return true;
	}

	int DepthMap::getWidth()
	{
		return m_params.m_width;
	}
	int DepthMap::getHeight()
	{
		return m_params.m_height;
	}
}