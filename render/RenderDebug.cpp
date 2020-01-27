#include "../Globals.h"
#include "RenderDebug.h"
#include "../glm/gtx/transform.hpp"
#include "Camera.h"

using namespace demomath;

namespace
{
	void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION: g_debug << "OpenGL debug notification: " << msg << std::endl; break;
			case GL_DEBUG_SEVERITY_LOW:	g_debug << "OpenGL debug low: " << msg << std::endl; break;
			case GL_DEBUG_SEVERITY_MEDIUM: g_debug << "OpenGL debug medium: " << msg << std::endl; break;
			case GL_DEBUG_SEVERITY_HIGH: g_debug << "OpenGL debug high: " << msg << std::endl; break;
		}
	}
}

namespace demorender
{
	RenderDebug* RenderDebug::sm_instance = nullptr;

	RenderDebug::RenderDebug()
	{
		initCaps();
	}

	RenderDebug::~RenderDebug()
	{

	}

	void RenderDebug::init()
	{
		int width, height;
		democore::g_system->getWindowSize(width, height);

		m_pOrtho = new OrthoCamera(0.f, width * 1.f, 0.f, height * 1.f);

		m_pSquare = new Model();
		m_pSquare->setMesh("square");

		m_pFont = new Font();
		m_pFont->load("arial.ttf");
	}

	RenderDebug* RenderDebug::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new RenderDebug();
		}
		return sm_instance;
		
	}

	const RenderCaps RenderDebug::getCaps() 
	{
		return m_caps;
	}

	void RenderDebug::initCaps()
	{
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_caps.maxVertexTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_caps.maxFragmentTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_caps.maxTextureSize);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &m_caps.max3DTextureSize);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &m_caps.maxCubemapTextureSize);
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_caps.maxDrawBuffers);
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &m_caps.maxUniformLocations);
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &m_caps.maxVaryingFloats);
		glGetIntegerv(GL_MAX_VARYING_VECTORS, &m_caps.maxVaryingVectors);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.maxVertexAttribs);

		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &m_caps.maxGeometryShaderOutputVertices);
		glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &m_caps.maxGeometryShaderTotalOutputComponents);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &m_caps.maxWorkGroupCountX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &m_caps.maxWorkGroupCountY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &m_caps.maxWorkGroupCountZ);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &m_caps.maxWorkGroupSizeX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &m_caps.maxWorkGroupSizeY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &m_caps.maxWorkGroupSizeZ);

		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &m_caps.maxWorkGroupInvocations);

		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &m_caps.maxShaderStorageBlockSize);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &m_caps.maxShaderStorageBufferBindings);
		glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &m_caps.maxVertexShaderStorageBlocks);
		glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &m_caps.maxFragmentShaderStorageBlocks);
		glGetIntegerv(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &m_caps.maxGeometryShaderStorageBlocks);
		glGetIntegerv(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, &m_caps.maxTessControlShaderStorageBlocks);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, &m_caps.maxTessEvalShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &m_caps.maxComputeShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &m_caps.maxCombinedShaderStoragelocks);
				
		g_debug << "OpenGL caps:" << std::endl;
		g_debug << "  max vertex texture units: " << m_caps.maxVertexTextureUnits << std::endl;
		g_debug << "  max fragment texture units: " << m_caps.maxFragmentTextureUnits << std::endl;
		g_debug << "  max texture size: " << m_caps.maxTextureSize << std::endl;
		g_debug << "  max cubemap size: " << m_caps.maxCubemapTextureSize << std::endl;
		g_debug << "  max 3D texture size: " << m_caps.max3DTextureSize << std::endl;
		g_debug << "  max draw buffers: " << m_caps.maxDrawBuffers << std::endl;
		g_debug << "  max uniform locations: " << m_caps.maxUniformLocations << std::endl;
		g_debug << "  max varying floats: " << m_caps.maxVaryingFloats << std::endl;
		g_debug << "  max varying vectors: " << m_caps.maxVaryingVectors << std::endl;
		g_debug << "  max vertex attribs: " << m_caps.maxVertexAttribs << std::endl;

		g_debug << "  max geometry shader output vertices " << m_caps.maxGeometryShaderOutputVertices << std::endl;
		g_debug << "  max geometry shader output components " << m_caps.maxGeometryShaderTotalOutputComponents << std::endl;

		g_debug << "  max compute work group count: " << m_caps.maxWorkGroupCountX << ", " << m_caps.maxWorkGroupCountY << ", " << m_caps.maxWorkGroupCountZ << std::endl;
		g_debug << "  max compute work group size: " << m_caps.maxWorkGroupSizeX << ", " << m_caps.maxWorkGroupSizeY << ", " << m_caps.maxWorkGroupSizeZ << std::endl;
		g_debug << "  max compute work group invocations: " << m_caps.maxWorkGroupInvocations << std::endl;
		g_debug << "  max shader storage block size: " << m_caps.maxShaderStorageBlockSize << std::endl;
		g_debug << "  max shader storage buffer bindings: " << m_caps.maxShaderStorageBufferBindings << std::endl;
		g_debug << "  max vertex shader storage blocks: " << m_caps.maxVertexShaderStorageBlocks << std::endl;
		g_debug << "  max fragment shader storage blocks: " << m_caps.maxFragmentShaderStorageBlocks << std::endl;
		g_debug << "  max geometry shader storage blocks: " << m_caps.maxGeometryShaderStorageBlocks << std::endl;
		g_debug << "  max tess control shader storage blocks: " << m_caps.maxTessControlShaderStorageBlocks << std::endl;
		g_debug << "  max tess eval shader storage blocks: " << m_caps.maxTessEvalShaderStorageBlocks << std::endl;
		g_debug << "  max compute shader storage blocks: " << m_caps.maxComputeShaderStorageBlocks << std::endl;
		g_debug << "  max combined shader storage blocks: " << m_caps.maxCombinedShaderStoragelocks << std::endl;

	}

	void RenderDebug::drawString(const std::string& str, int x, int y, float scale, glm::vec3 color)
	{
		m_pFont->drawString(str, x, y, scale, color);


	}

	void RenderDebug::resetGL()
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void RenderDebug::drawTexture(const std::string& name, int x, int y, float scale)
	{
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("simpletexture");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(name, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(x * 1.f, y * 1.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;
		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderDebug::drawTexture(GLuint textureId, int x, int y, float scale)
	{
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("simpletexture");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		g_textures->bindTexture(textureId, GL_TEXTURE0); GL_DEBUG;
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(x * 1.f, y * 1.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;
		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderDebug::drawDepthTexture(GLuint textureId, Camera* pCamera, int x, int y, float scale)
	{
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("debug_depthtexture");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		glBindTexture(GL_TEXTURE_2D, textureId);
		shader.setUniform1i("tex", 0); GL_DEBUG;
		shader.setUniform1f("near", pCamera->getNearPlane());
		shader.setUniform1f("far", pCamera->getFarPlane());

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(x * 1.f, y * 1.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;
		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void RenderDebug::drawDepthTextureOrtho(GLuint textureId, int x, int y, float scale)
	{
		glm::mat4 testiTrans = glm::scale(glm::vec3(1.f));

		Shader& shader = g_shaders->getShader("debug_depthtextureortho");
		shader.bind();
		shader.setUniformMatrix4fv("camera", 1, GL_FALSE, (float *)&m_pOrtho->getCameraMatrix()); GL_DEBUG;
		m_pSquare->bind(&shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		shader.setUniform1i("tex", 0); GL_DEBUG;

		float aspect = democore::g_system->getAspectRatio();
		glm::mat4 testiModelMatrix = glm::translate(x * 1.f, y * 1.f, 0.f);
		glm::mat4 scaling = glm::scale(glm::vec3(scale, scale, 1.f));
		testiModelMatrix *= scaling;
		shader.setUniformMatrix4fv("model", 1, GL_FALSE, (float *)&testiModelMatrix); GL_DEBUG;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void RenderDebug::setDebugging(bool debugging)
	{
		m_debugging = debugging;
		if (debugging)
		{
			GLint flags;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
			{
				g_debug << "Error! context does not support debugging!" << std::endl;
				m_debugging = false;
				return;
			}
			glDebugMessageCallback(&debugCallback, NULL);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
//			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
//			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
//			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_FALSE);
		}
	}

	void RenderDebug::logError(const std::string& text)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::stringstream ss;
			ss << text << " : ";
			switch (err)
			{
				case GL_INVALID_ENUM: ss << "GL_INVALID_ENUM" << std::endl; break;
				case GL_INVALID_VALUE: ss << "GL_INVALID_VALUE" << std::endl; break;
				case GL_INVALID_OPERATION: ss << "GL_INVALID_OPERATION" << std::endl; break;
				case GL_STACK_OVERFLOW: ss << "GL_STACK_OVERFLOW" << std::endl; break;
				case GL_STACK_UNDERFLOW: ss << "GL_STACK_UNDERFLOW" << std::endl; break;
				case GL_OUT_OF_MEMORY: ss << "GL_OUT_OF_MEMORY" << std::endl; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: ss << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
				case GL_CONTEXT_LOST: ss << "GL_CONTEXT_LOST" << std::endl; break;
				case GL_TABLE_TOO_LARGE: ss << "GL_TABLE_TOO_LARGE" << std::endl; break;
				default: ss << "glGetError() returned unspecified error code " << err << std::endl;
			}
			g_debug << ss.str();
		}
	}
	void RenderDebug::logError(const std::string& fileName, int line)
	{
		std::stringstream ss;
		ss << fileName << "(" << line << ")";
		logError(ss.str());
	}
}