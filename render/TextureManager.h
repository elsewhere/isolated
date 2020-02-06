#pragma once

#include "Texture.h"
#include "Image.h"

#define g_textures TextureManager::inst()

namespace demorender
{
	class TextureParameters;
	class Texture;
	class Cubemap;
	class DepthMap;

	class TextureManager
	{
	public:
		void init();
		void clear();

		static TextureManager* inst();
		std::map<std::string, std::unique_ptr<TextureParameters>>* getAllTextureParameters() { return &m_textureParameters; };

		//textures
		Texture* getTexture(const std::string& name);
		Cubemap* getCubemap(const std::string& name);
		DepthMap* getDepthMap(const std::string& name);
		void addTextureParameters(const std::string& name, TextureParameters* params);
		void loadImages();
		void createTextures();
		void bindTexture(const std::string& name, int texunit = GL_TEXTURE0_ARB);
		void bindTexture(GLuint textureId, int texunit = GL_TEXTURE0_ARB);
		void bindTextureToImage(const std::string& name, GLuint unit, GLenum access);
		void bindCubemap(const std::string& name, int texunit = GL_TEXTURE0_ARB);
		void bindCubemap(GLuint textureId, int texunit = GL_TEXTURE0_ARB);
		void clearTextureUnits(); //for lazy programmers

		//images
		void addImage(const std::string& name, Image *image);
		Image* image(const std::string& name);

		//debug
		void dumpUnusedImages();


	private:
		TextureManager();
		~TextureManager();

		static const int MAX_TEXTURES = 8;
		std::map<std::string, std::unique_ptr<Image>> m_images;
		std::map<std::string, std::unique_ptr<Texture>> m_textures;
		std::map<std::string, std::unique_ptr<Cubemap>> m_cubemaps;
		std::map<std::string, std::unique_ptr<DepthMap>> m_depthmaps;
		std::map<std::string, std::unique_ptr<TextureParameters>> m_textureParameters;
		std::string m_lastBoundTexture[8];


		static TextureManager* sm_instance;

	};
}