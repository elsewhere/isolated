#pragma once

#include "../Globals.h"

#define g_renderTargets RenderTargetManager::inst()

namespace demorender
{
	class RenderTargetManager
	{
	public:
		static RenderTargetManager* inst();

		void bindMain();
		void bindTexture(const std::string& textureName);
		void bindCubemapSide(const std::string& cubemapName, int side);
		GLuint getDepthTextureId(const std::string& textureName);
		void unbind();

	private:

		class TextureFBO
		{
		public:
			TextureFBO(class Texture *pTexture);
			~TextureFBO();
			bool init();
			void bind();

			GLuint getDepthTextureId();
		private:
			GLuint m_frameBuffer;
			GLuint m_textureId;
			GLuint m_depthRenderBuffer;
			GLuint m_depthTextureId;
			int m_width, m_height;
		};

		class CubemapFBO
		{
		public:
			CubemapFBO(class Cubemap *pCubemap);
			~CubemapFBO();
			bool init();
			void bindSide(int side);

//			GLuint getDepthTextureId();
		private:
			GLuint m_textureId;
			GLuint m_frameBuffer[6];
			GLuint m_depthRenderBuffer[6];
			GLuint m_depthTextureId[6];
			int m_width, m_height;
		};

		static bool checkErrors(const std::string& str);


		RenderTargetManager();
		~RenderTargetManager();

		static RenderTargetManager* sm_instance;

		std::map<int, TextureFBO*> m_textureFbos;
		std::map<int, CubemapFBO*> m_cubemapFbos;
		int textureHash(class Texture *pTexture);
		int cubemapHash(class Cubemap *pCubemap);
	};
}