#include "TextureManager.h"

namespace
{
	const bool EXTRA_DEBUG = false;
}
namespace demorender
{
	TextureManager* TextureManager::sm_instance = nullptr;


	TextureManager::TextureManager()
	{
	}
	TextureManager::~TextureManager()
	{
	}

	Texture *TextureManager::getTexture(const std::string& name)
	{
		if (m_textures.find(name) != m_textures.end())
		{
			return m_textures[name].get();
		}
		else
		{
			g_debug << "could not find texture " << name << std::endl;
			return nullptr;
		}
	}

	Cubemap *TextureManager::getCubemap(const std::string& name)
	{
		if (m_cubemaps.find(name) != m_cubemaps.end())
		{
			return m_cubemaps[name].get();
		}
		else
		{
			g_debug << "could not find cubemap " << name << std::endl;
			return nullptr;
		}
	}

	TextureManager* g_textures
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new TextureManager();
		}
		return sm_instance;
	}

	void TextureManager::init()
	{
		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			m_lastBoundTexture[i] = "no texture";
		}
	}
	void TextureManager::clear()
	{
		m_images.clear();
		m_textures.clear();
		m_cubemaps.clear();
		m_textureParameters.clear();

		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			m_lastBoundTexture[i] = "no texture";
		}
	}

	void TextureManager::addTextureParameters(const std::string& name, TextureParameters* params)
	{
		if (m_textureParameters.find(name) == m_textureParameters.end())
		{
			m_textureParameters[name] = std::make_unique<TextureParameters>(*params);
		}
		else
		{
			g_debug << "trying to add duplicate textureparams : " << name << std::endl;
		}
	}

	void TextureManager::addImage(const std::string& name, Image *image)
	{
		if (EXTRA_DEBUG)
		{
			g_debug << "adding image " << name << std::endl;
		}
		if (m_images.find(name) == m_images.end())
		{
			m_images[name] = std::make_unique<Image>(*image);
		}
		else
		{
			g_debug << "trying to add duplicate image : " << name << std::endl;
		}

	}
	void TextureManager::bindTexture(const std::string& name, int texunit)
	{
		int texunitoffset = texunit - GL_TEXTURE0_ARB;
		if (texunitoffset < 0 || texunitoffset > MAX_TEXTURES)
		{
			std::stringstream ss;
			ss << "trying to bind texture " << name << " to an invalid texture unit << " << texunitoffset << "!";
			g_debug << ss.str() << std::endl;
			g_error.log(ss.str());
			return;
		}
		if (m_textures.find(name) != m_textures.end())
		{
			m_textures[name]->bind(texunit);
			m_lastBoundTexture[texunitoffset] = name;
			RenderStatistics::sm_texturesBound++;
		}
		else
		{
			std::stringstream ss;
			ss << "trying to bind texture " << name << " that does not exist!";
			g_debug << ss.str() << std::endl;
			g_error.log(ss.str());
		}
	}

	void TextureManager::bindTexture(GLuint textureId, int texunit)
	{
		int texunitoffset = texunit - GL_TEXTURE0_ARB;
		if (texunitoffset < 0 || texunitoffset > MAX_TEXTURES)
		{
			std::stringstream ss;
			ss << "trying to bind texture with textureID " << textureId << " to an invalid texture unit << " << texunitoffset << "!";
			g_debug << ss.str() << std::endl;
			g_error.log(ss.str());
			return;
		}

		RenderStatistics::sm_texturesBound++;
		glActiveTexture(texunit);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}


	void TextureManager::bindTextureToImage(const std::string& name, GLuint unit, GLenum access)
	{
		if (m_textures.find(name) != m_textures.end())
		{

			glBindImageTexture(unit, m_textures[name]->getID(), 0, GL_FALSE, 0, access, m_textures[name]->getFormat());
		}
		else
		{
			std::stringstream ss;
			ss << "trying to bind to image texture " << name << " that does not exist!";
			g_debug << ss.str() << std::endl;
			g_error.log(ss.str());

		}

	}
	void TextureManager::bindCubemap(const std::string& name, int texunit)
	{
		int texunitoffset = texunit - GL_TEXTURE0_ARB;
		if (texunitoffset < 0 || texunitoffset > MAX_TEXTURES)
		{
			g_debug << "trying to bind cubemap " << name << " to an invalid texture unit << " << texunitoffset << "!" << std::endl;
			return;
		}
		if (m_cubemaps.find(name) != m_cubemaps.end())
		{
			m_cubemaps[name]->bind(texunit);
			m_lastBoundTexture[texunitoffset] = name;
			RenderStatistics::sm_texturesBound++;
		}
		else
		{
			g_debug << "trying to bind cubemap " << name << " that does not exist!" << std::endl;
		}
	}


	void TextureManager::clearTextureUnits()
	{
		//this is slow but what the hell... 
		glActiveTexture(GL_TEXTURE7_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE6_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE5_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE4_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE3_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1_ARB);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0_ARB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDisable(GL_TEXTURE_2D);
	}
	void TextureManager::loadImages()
	{
		int startTime = timeGetTime();
		std::vector<std::string> filenames;
		std::string directory = "data\\graphics\\";
		StringUtils::iterateDirectory(directory, filenames);

		//add steps, one for uploading and one for loading
		democore::g_system->addLoadingScreenSteps(filenames.size() * 2);

		for (std::string& filename : filenames)
		{
			std::string path = directory + filename;
			std::string suffix = filename.substr(filename.length()-3, filename.length());

			if (suffix == "jpg")
			{
				Image *image = ImageFactory::loadJPG(path);
				if (image != 0)
				{
					addImage(filename, image);
				}
				democore::g_system->advanceLoadingScreen(1);
			}
			else if (suffix == "png")
			{
				Image *image = ImageFactory::loadPNG(path);
				if (image != 0)
				{
					addImage(filename, image);
				}
				democore::g_system->advanceLoadingScreen(1);
			}
			else
			{
				g_debug << "non-image file " << filename << " found in graphics directory!" << std::endl;
			}
			democore::g_system->drawLoadingScreen();
		}
		int endTime = timeGetTime();
		g_debug << "image loading took " << (endTime - startTime) << " ms\n";

	}

	void TextureManager::createTextures()
	{
		int textureCount = 0;
		g_debug << "uploading textures to OpenGL" << std::endl;
		int videoMemoryConsumption = 0;
		int startTime = timeGetTime();

		//iterate through everything in textureparameters and create stuff accordingly
		for (const auto& param : m_textureParameters)
		{
			auto pParams = param.second.get();

			if (pParams->m_type == TextureParameters::TEXTURE_2D)
			{
				g_debug << "creating 2d texture " << param.first << std::endl;
				Image *pImage = 0;
				if (pParams->m_filename != "invalid")
				{
					//there is (should be) image data available with this name
					pImage = image(pParams->m_filename);
					if (pImage == 0)
					{
						//error
						g_debug << "Cannot create texture! Missing image file " << pParams->m_filename << std::endl;
					}
				}

//				std::unique_ptr<Texture> texture = std::make_unique<Texture>(*pParams, pImage);
				auto texture = std::make_unique<Texture>();

				if (texture->create(*pParams, pImage))
				{
					int texels = texture->getWidth() * texture->getHeight();

					switch (pParams->m_format)
					{
						case TextureParameters::RGBA: videoMemoryConsumption += texels * 4; break;
						case TextureParameters::FLOAT16: videoMemoryConsumption += texels * 4 * 2; break;
						case TextureParameters::FLOAT32: videoMemoryConsumption += texels * 4 * 4; break;
						default: videoMemoryConsumption += texels * 4;
					}
					m_textures[param.first] = std::move(texture);

					textureCount++;
				}
				else
				{
					g_debug << "texture create failed " << param.first << std::endl;
				}

			}
			else if (pParams->m_type == TextureParameters::CUBEMAP)
			{
				g_debug << "creating cube map " << param.first << std::endl;
				std::vector<Image*> images;
				std::unique_ptr<Cubemap> cubemap = std::make_unique<Cubemap>();
				
				//if all filenames have been specified, get images
				if (pParams->m_filenamePosX != "invalid" &&
					pParams->m_filenameNegX != "invalid" &&
					pParams->m_filenamePosY != "invalid" &&
					pParams->m_filenameNegY != "invalid" &&
					pParams->m_filenamePosZ != "invalid" &&
					pParams->m_filenameNegZ != "invalid")
				{
					Image *posX = image(pParams->m_filenamePosX);
					Image *negX = image(pParams->m_filenameNegX);
					Image *posY = image(pParams->m_filenamePosY);
					Image *negY = image(pParams->m_filenameNegY);
					Image *posZ = image(pParams->m_filenamePosZ);
					Image *negZ = image(pParams->m_filenameNegZ);

					//mwahahaha, every project needs a goto :) 
					if (posX == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenamePosX << " for cubemap " << param.first << std::endl;
						goto fail;
					}
					if (negX == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenameNegX << " for cubemap " << param.first << std::endl;
						goto fail;
					}
					if (posY == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenamePosY << " for cubemap " << param.first << std::endl;
						goto fail;
					}
					if (negY == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenameNegY << " for cubemap " << param.first << std::endl;
						goto fail;
					}

					if (posZ == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenamePosZ << " for cubemap " << param.first << std::endl;
						goto fail;
					}
					if (negZ == 0)
					{
						g_debug << "could not load image file " << pParams->m_filenameNegZ << " for cubemap " << param.first << std::endl;
						goto fail;
					}
					//all of them are valid
					images.push_back(posX);
					images.push_back(negX);
					images.push_back(posY);
					images.push_back(negY);
					images.push_back(posZ);
					images.push_back(negZ);

					goto done;

					fail:
						images.clear();
				}
				done:
				if (cubemap->create(*pParams, images))
				{
					int texels = cubemap->getWidth() * cubemap->getHeight() * 6;

					switch (pParams->m_format)
					{
						case TextureParameters::RGBA: videoMemoryConsumption += texels * 4; break;
						case TextureParameters::FLOAT16: videoMemoryConsumption += texels * 4 * 2; break;
						case TextureParameters::FLOAT32: videoMemoryConsumption += texels * 4 * 4; break;
						default: videoMemoryConsumption += texels * 4;
					}
					m_cubemaps[param.first] = std::move(cubemap);
					textureCount++;
				}
				else
				{
					g_debug << "cubemap create failed " << param.first << std::endl;
				}

			}
		}
		int endTime = timeGetTime();
		g_debug << "texture creation took " << (endTime - startTime) << " ms\n";

	}

	Image* TextureManager::image(const std::string& name)
	{
		if (m_images.find(name) == m_images.end())
		{
			return nullptr;
		}
		else
		{
			return m_images[name].get();
		}
	}

	void TextureManager::dumpUnusedImages()
	{
		g_debug << "" << std::endl;
		g_debug << "TextureManager::dumpUnusedImages()" << std::endl;
		g_debug << "-------------------" << std::endl;
		
		for (auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			Texture *t = it->second.get();
			if (!t->hasBeenUsed())
			{
				g_debug << "  unused texture: " << (*it).first << std::endl;
			}
		}
		for (auto it2 = m_cubemaps.begin(); it2 != m_cubemaps.end(); it2++)
		{
			Cubemap *t = (*it2).second.get();
			if (!t->hasBeenUsed())
			{
				g_debug << "  unused cubemap: " << (*it2).first << std::endl;
			}
		}
		g_debug << "" << std::endl;
	}

}