#pragma once

#include "../Globals.h"

#define GL_DEBUG RenderDebug::logError(__FILE__, __LINE__);
#define g_renderDebug RenderDebug::inst()

namespace demorender
{
	
	class RenderCaps
	{
	public:
		int maxVertexTextureUnits;
		int maxFragmentTextureUnits;
		int maxTextureSize;
		int max3DTextureSize;
		int maxCubemapTextureSize;
		int maxDrawBuffers;
		int maxUniformLocations;
		int maxVaryingFloats;
		int maxVaryingVectors;
		int maxVertexAttribs;
		int maxWorkGroupCountX;
		int maxWorkGroupCountY;
		int maxWorkGroupCountZ;
		int maxWorkGroupSizeX;
		int maxWorkGroupSizeY;
		int maxWorkGroupSizeZ;
		int maxWorkGroupInvocations;

		int maxGeometryShaderOutputVertices;
		int maxGeometryShaderTotalOutputComponents;

		int maxShaderStorageBlockSize;
		int maxShaderStorageBufferBindings;
		int maxVertexShaderStorageBlocks;
		int maxFragmentShaderStorageBlocks;
		int maxGeometryShaderStorageBlocks;
		int maxTessControlShaderStorageBlocks;
		int maxTessEvalShaderStorageBlocks;
		int maxComputeShaderStorageBlocks;
		int maxCombinedShaderStoragelocks;


	};

	class RenderDebug
	{
	public:
		static void logError(const std::string& text);
		static void logError(const std::string& fileName, int line);

		static RenderDebug* inst();

		void setDebugging(bool debugging);

		const RenderCaps getCaps();

		void resetGL();
		void drawString(const std::string& str, int x, int y, float scale, glm::vec3 color);
		void drawTexture(const std::string& name, int x, int y, float scale = 1.f);
		void drawTexture(const GLuint textureId, int x, int y, float scale = 1.f);
		void drawDepthTexture(GLuint textureId, class Camera *pCamera, int x, int y, float scale = 1.f);

	private:
		RenderDebug();
		~RenderDebug();

		void initCaps();
		RenderCaps m_caps;

		static RenderDebug* sm_instance;

		class OrthoCamera* m_pOrtho = nullptr;
		class Model *m_pSquare = nullptr;
		class Font *m_pFont = nullptr;

		bool m_debugging = false;

	};
}