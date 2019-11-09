#include "../Globals.h"
#include "RenderTargetManager.h"

using namespace demomath;

namespace demorender
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TextureFBO
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderTargetManager::TextureFBO::TextureFBO(Texture *pTexture) :m_frameBuffer(GL_INVALID_VALUE)
		,m_textureId(GL_INVALID_VALUE)
		,m_depthRenderBuffer(GL_INVALID_VALUE)
		,m_depthTextureId(GL_INVALID_VALUE)
		,m_width(0)
		,m_height(0)
	{
		if (pTexture)
		{
			m_width = pTexture->getWidth();
			m_height = pTexture->getHeight();
			m_textureId = pTexture->getID();
		}
		else
		{
			g_debug << "trying to create a textureFBO with a null texture!" << std::endl;
		}
	}

	RenderTargetManager::TextureFBO::~TextureFBO()
	{

	}

	bool RenderTargetManager::TextureFBO::init()
	{
		if (m_textureId == GL_INVALID_VALUE)
		{
			g_debug << "Trying to initialize a TextureFBO for an invalid texture" << std::endl;
			return false;
		}
		if (m_width <= 0 || m_height <= 0)
		{
			g_debug << "invalid dimensions for a rendertarget! width = " << m_width << " height = " << m_height << std::endl;
			return false;
		}

		//create framebuffer
		glGenFramebuffers(1, &m_frameBuffer); GL_DEBUG;
		RenderTargetManager::checkErrors("framebuffer create");

		//create depth render buffer
		glGenRenderbuffers(1, &m_depthRenderBuffer); GL_DEBUG;
		RenderTargetManager::checkErrors("renderbuffer create");

		//create depth texture
		glGenTextures(1, &m_depthTextureId);
		glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		//set texture parameters for the framebuffer texture
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer); GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, m_textureId); GL_DEBUG;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GL_DEBUG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GL_DEBUG;

		//create a renderbuffer for depth
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer); GL_DEBUG;
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height); GL_DEBUG;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer); GL_DEBUG;
		RenderTargetManager::checkErrors("create depth renderbuffer");

		//attach textures
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0); GL_DEBUG;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureId, 0);
		RenderTargetManager::checkErrors("framebuffer attach textures - then done!");

//		g_debug << "frame buffer = " << m_frameBuffer << " depth texture = " << m_depthTextureId << " depth render buffer = " << m_depthRenderBuffer << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	void RenderTargetManager::TextureFBO::bind()
	{
//		glBindTexture(GL_TEXTURE_2D, 0);
//		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_width, m_height);
	}

	GLuint RenderTargetManager::TextureFBO::getDepthTextureId()
	{
		return m_depthTextureId;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CubemapFBO
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	RenderTargetManager::CubemapFBO::CubemapFBO(Cubemap *pCubemap) :m_textureId(GL_INVALID_VALUE)
		, m_width(0)
		, m_height(0)
	{
		if (pCubemap)
		{
			m_width = pCubemap->getWidth();
			m_height = pCubemap->getHeight();
			m_textureId = pCubemap->getID();
		}
		else
		{
			g_debug << "trying to create a cubemapFBO with a null cubemap!" << std::endl;
		}
		for (int i = 0; i < 6; i++)
		{
			m_frameBuffer[i] = GL_INVALID_VALUE;
			m_depthRenderBuffer[i] = GL_INVALID_VALUE;
			m_depthTextureId[i] = GL_INVALID_VALUE;
		}
	}

	RenderTargetManager::CubemapFBO::~CubemapFBO()
	{

	}

	bool RenderTargetManager::CubemapFBO::init()
	{
		if (m_textureId == GL_INVALID_VALUE)
		{
			g_debug << "Trying to initialize a CubemapFBO for an invalid cubemap" << std::endl;
			return false;
		}
		if (m_width <= 0 || m_height <= 0)
		{
			g_debug << "invalid dimensions for a cubemap rendertarget! width = " << m_width << " height = " << m_height << std::endl;
			return false;
		}

		//create framebuffers
		glGenFramebuffers(6, m_frameBuffer); GL_DEBUG;
		RenderTargetManager::checkErrors("framebuffer create");
		for (int i = 0; i < 6; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer[i]); GL_DEBUG;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_textureId, 0);
			//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeDepthTex, 0);

			std::stringstream ss;
			ss << "cube framebuffer create i = " << i;
			RenderTargetManager::checkErrors(ss.str());
		}

		//create depth render buffer
//		glGenRenderbuffers(1, &m_depthRenderBuffer); GL_DEBUG;
//		RenderTargetManager::checkErrors("renderbuffer create");

		//create depth texture
//		glGenTextures(1, &m_depthTextureId);
//		glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
//		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		//set texture parameters for the framebuffer texture
//		glBindTexture(GL_TEXTURE_2D, m_textureId); GL_DEBUG;

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GL_DEBUG;
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GL_DEBUG;

		//create a renderbuffer for depth
//		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer); GL_DEBUG;
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height); GL_DEBUG;
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer); GL_DEBUG;
//		RenderTargetManager::checkErrors("create depth renderbuffer");

		//attach textures
//		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0); GL_DEBUG;
//		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureId, 0);
//		RenderTargetManager::checkErrors("framebuffer attach textures - then done!");

		//		g_debug << "frame buffer = " << m_frameBuffer << " depth texture = " << m_depthTextureId << " depth render buffer = " << m_depthRenderBuffer << std::endl;
//		glBindTexture(GL_TEXTURE_2D, 0);

		RenderTargetManager::checkErrors("cubemap init final step!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	void RenderTargetManager::CubemapFBO::bindSide(int side)
	{
		//		glBindTexture(GL_TEXTURE_2D, 0);
		//		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer[side]);
		glViewport(0, 0, m_width, m_height);
	}


	RenderTargetManager* RenderTargetManager::sm_instance = nullptr;

	RenderTargetManager::RenderTargetManager()
	{
	}
	RenderTargetManager::~RenderTargetManager()
	{

	}

	RenderTargetManager* RenderTargetManager::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new RenderTargetManager();
		}
		return sm_instance;	
	}

	int RenderTargetManager::textureHash(Texture *pTexture)
	{
		return pTexture->getID(); //TODO: not enough, need xres yres etc. This will create a new TextureFBO for each texture. 

//		int width = pTexture->getWidth();
//		int height = pTexture->getHeight();

//		return width + height * 10000; //super awesome hash function!
	}

	int RenderTargetManager::cubemapHash(Cubemap *pCubemap)
	{
		return pCubemap->getID(); //TODO: not enough, need xres yres etc. This will create a new TextureFBO for each texture. 

								  //		int width = pTexture->getWidth();
								  //		int height = pTexture->getHeight();

								  //		return width + height * 10000; //super awesome hash function!
	}

	void RenderTargetManager::bindTexture(const std::string& textureName)
	{
		//find if proper TextureFBO exist
		Texture *pTexture = g_textures->getTexture(textureName);
		if (!pTexture)
		{
			g_debug << "Trying to bind rendertarget to texture " << textureName << " but it does not exist!" << std::endl;
			return;
		}
		else
		{
			int hash = textureHash(pTexture);
			if (m_textureFbos.find(hash) == m_textureFbos.end())
			{
				//framebuffer object does not exist, create a new one that matches this texture 
				TextureFBO* TextureFBO = new RenderTargetManager::TextureFBO(pTexture);
				if (TextureFBO->init())
				{
					g_debug << "created a new TextureFBO for texture " << textureName << std::endl;
					m_textureFbos[hash] = TextureFBO;
				}
				else
				{
					g_debug << "could not initialize TextureFBO for texture " << textureName << std::endl;
					return;
				}
			}
			m_textureFbos[hash]->bind();
		}
	}

	void RenderTargetManager::bindCubemapSide(const std::string& cubemapName, int side)
	{
		Cubemap *pCubemap = g_textures->getCubemap(cubemapName);
		if (!pCubemap)
		{
			g_debug << "Trying to bind rendertarget to cubemap " << cubemapName << " but it does not exist!" << std::endl;
			return;
		}
		else
		{
			int hash = cubemapHash(pCubemap);
			if (m_cubemapFbos.find(hash) == m_cubemapFbos.end())
			{
				//framebuffer object does not exist, create a new one that matches this texture 
				CubemapFBO* cubemapFBO = new RenderTargetManager::CubemapFBO(pCubemap);
				if (cubemapFBO->init())
				{
					g_debug << "created a new cubemapFBO for cubemao " << cubemapName << std::endl;
					m_cubemapFbos[hash] = cubemapFBO;
				}
				else
				{
					g_debug << "could not initialize cubemapFBO for cubemap" << cubemapName << std::endl;
					return;
				}
			}
			m_cubemapFbos[hash]->bindSide(side);
		}
	}


	GLuint RenderTargetManager::getDepthTextureId(const std::string& textureName)
	{
		Texture *pTexture = g_textures->getTexture(textureName);
		if (!pTexture)
		{
			g_debug << "Trying to get depth texture id for rendertarget/texture " << textureName << " but texture is null!" << std::endl;
			return GL_INVALID_VALUE;
		}
		int hash = textureHash(pTexture);
		if (m_textureFbos.find(hash) == m_textureFbos.end())
		{
			g_debug << "TextureFBO for texture " << textureName << " has not been created!" << std::endl;
			return GL_INVALID_VALUE;
		}
		else
		{
			return m_textureFbos[hash]->getDepthTextureId();
		}
	}

	void RenderTargetManager::bindMain()
	{
		bindTexture("main");
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}
	void RenderTargetManager::unbind()
	{
		int width, height;
		democore::g_system->getWindowSize(width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glViewport(0, 0, width, height);

	}

	bool RenderTargetManager::checkErrors(const std::string& str)
	{
		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status)
		{
			case GL_FRAMEBUFFER_COMPLETE:
				//alles ok 
				return true;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				g_debug << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER - " << str << std::endl;
				return false;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				g_debug << "GL_FRAMEBUFFER_UNSUPPORTED - " << str << std::endl;
				break;

			default:
				g_debug << "Something really fucked in TextureFBO initialization - " << str << std::endl;
				break;
		}
		return true;
	}

}