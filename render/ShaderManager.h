#pragma once

#include "Shader.h"

#define g_shaders ShaderManager::inst()

namespace demorender
{
	class ShaderInfo;
	class Shader;

	class ShaderManager
	{
	public:
		static ShaderManager *inst();

		void bindShader(const std::string& name);
		Shader& getShader(const std::string& name); 
		void unbindShader();
		void loadShaders(); 
		void freeShaders();
		void dumpUnusedShaders();
		void setBound(Shader *shader); //kludge but... 

	private:
		ShaderManager();
		~ShaderManager();

		static ShaderManager *sm_instance;

		bool parseShaderFile(const std::string& filename, ShaderInfo& info);
		Shader *m_currentlyBound;
		std::map<std::string, Shader*> m_shaders;
	};
}