#include <numeric>
#include "ShaderManager.h"

namespace demorender
{
	ShaderManager* ShaderManager::sm_instance = nullptr;

	ShaderManager::ShaderManager()
	{
		m_currentlyBound = 0;
	}
	ShaderManager::~ShaderManager()
	{
	}

	ShaderManager* ShaderManager::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new ShaderManager();
		}
		return sm_instance;
	}


	void ShaderManager::setBound(Shader *shader)
	{
		m_currentlyBound = shader;
	} 

	void ShaderManager::bindShader(const std::string& name)
	{
		m_shaders[name]->bind();
	}

	void ShaderManager::unbindShader()
	{
		if (m_currentlyBound != 0)
		{
			m_currentlyBound->setBound(false);
			m_currentlyBound = 0;
		}
		else
		{
//			g_debug << "trying to unbind a shader when none are bound!" << endl;
		}
		glUseProgram(0);
	}

	Shader& ShaderManager::getShader(const std::string& name)
	{
		if (m_shaders.find(name) == m_shaders.end())
		{
			g_debug << "Cannot find shader " << name << "!" << std::endl;
		}
		return *m_shaders[name];
	}

	void ShaderManager::loadShaders()
	{
		int shaderIndex = 0;
		std::string shaderPath = "data\\shaders\\";
		std::string filter = "*.shader";

		std::vector<std::string> shaderDirectories;
		shaderDirectories.push_back("data\\shaders\\");
		shaderDirectories.push_back("data\\shaders\\postprocess\\");
		shaderDirectories.push_back("data\\shaders\\test\\");
		shaderDirectories.push_back("data\\shaders\\debug\\");

		g_debug << "" << std::endl;
		g_debug << "loadShaders()" << std::endl;
		g_debug << "-------------" << std::endl;

		for (auto &directory : shaderDirectories)
		{
			std::string path = directory + filter;
			std::vector<std::string> shaderFilenames;

			g_debug << "path " << path << std::endl;

			//fetch the list of shader files
			StringUtils::iterateDirectory(path, shaderFilenames);

			for (auto filename : shaderFilenames)
			{
				std::string shaderName = filename.substr(0, filename.length() - 7);
				g_debug << "shader #" << shaderIndex << ": " << shaderName << std::endl;

				std::string shaderPath = directory + filename;
				ShaderInfo info;
				if (parseShaderFile(shaderPath, info))
				{
					Shader *shader = new Shader();

					shader->init(shaderName, info);
					m_shaders[shaderName] = shader;
				}
				else
				{
					g_debug << "could not load " << shaderName << " for some reason, won't work etc" << std::endl;
				}
				shaderIndex++;
			}
			g_debug << "" << std::endl;

		}
	}

	void ShaderManager::freeShaders()
	{
		for (auto& s : m_shaders)
		{
			s.second->release();
		}
		m_shaders.clear();
	}

	bool ShaderManager::parseShaderFile(const std::string& filename, ShaderInfo& info)
	{
		std::vector<std::string> buffer;
		if (StringUtils::readFile(filename, buffer, false))
		{
			std::vector<std::string> vertexResult;
			std::vector<std::string> fragmentResult;
			std::vector<std::string> geometryResult;
			std::vector<std::string> tessControlResult;
			std::vector<std::string> tessEvaluationResult;
			std::vector<std::string> computeResult;

			//parse path of the file from the filename in case it's needed for shader preprocessor
			std::string path = filename.substr(0, filename.find_last_of('\\') + 1);

			bool hasFragment = false;
			bool hasGeometry = false;
			bool hasTessellation = false;
			bool isCompute = false;

			if (StringUtils::readBlock(path, "COMPUTE_SHADER", buffer, computeResult))
			{
				isCompute = true;
				info.isCompute = true;
				for (const auto& s : computeResult)
					info.computeSource += s;
				return true;
			}
			else
			{
				std::vector<std::string> tempVaryings;
				if (StringUtils::readBlock(path, "FEEDBACK_VARYINGS", buffer, tempVaryings))
				{
					for (auto& s : tempVaryings)
					{
						StringUtils::removeChar(s, '\n');
						StringUtils::removeChar(s, '\t');
						StringUtils::removeChar(s, ' ');
						if (!s.empty())
						{
							info.feedbackVaryings.push_back(s);
							info.hasFeedbackVaryings = true;
						}
					}
				}

				if (!StringUtils::readBlock(path, "VERTEX_SHADER", buffer, vertexResult))
				{
					g_debug << "could not read vertex shader block from " << filename << std::endl;
					return false;
				}
				if (StringUtils::readBlock(path, "FRAGMENT_SHADER", buffer, fragmentResult))
				{
					hasFragment = true;
				}
				if (StringUtils::readBlock(path, "GEOMETRY_SHADER", buffer, geometryResult))
				{
					hasGeometry = true;
				}
				if (StringUtils::readBlock(path, "TESS_CONTROL", buffer, tessControlResult))
				{
					hasTessellation = true;
				}
				if (StringUtils::readBlock(path, "TESS_EVALUATION", buffer, tessEvaluationResult))
				{
					hasTessellation = true;
				}

				for (const auto& s : vertexResult)
					info.vertexSource += s;

				if (hasFragment)
				{
					for (const auto& s : fragmentResult)
						info.fragmentSource += s;
				}

				if (hasGeometry)
				{
					for (const auto& s : geometryResult)
						info.geometrySource += s;
				}
				if (hasTessellation)
				{
					for (const auto& s : tessControlResult)
						info.tessControlSource += s;
					for (const auto& s : tessEvaluationResult)
						info.tessEvaluationSource += s;
				}

				info.hasGeometry = hasGeometry;
				info.hasFragment = hasFragment;
				info.hasTessellation = hasTessellation;
			}
		}
		else
		{
			g_debug << "could not read file " << filename << " into buffer" << std::endl;
			return false;
		}

		return true;
	}

	void ShaderManager::dumpUnusedShaders()
	{
		g_debug << "" << std::endl;
		g_debug << "ShaderManager::dumpUnusedShaders()" << std::endl;
		g_debug << "-------------------" << std::endl;

		for (auto& s : m_shaders)
		{
			if (!s.second->hasBeenUsed())
			{
				g_debug << "  unused shader: " << s.first << std::endl;
			}
		}
		g_debug << "" << std::endl;
	}		
}