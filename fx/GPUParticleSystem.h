#pragma once

#include "../Globals.h"

namespace demofx
{
	class GPUParticleSystem
	{
		using ParticleUniform = std::variant<int, float, glm::vec3, glm::vec4, glm::mat4>;
		using UniformList = std::vector<std::pair<std::string, ParticleUniform>>;

		struct ParticleAttribute
		{
			std::string name;
			int size = 0;

			inline bool operator == (const ParticleAttribute& rhs)
			{
				return name == rhs.name && size == rhs.size;
			}
		};

	public:
		GPUParticleSystem(int particleCount);
		~GPUParticleSystem();

		virtual void setInitialData();
		void createBuffers();
		void update();
		void draw(demorender::Camera *pCamera);
		void setShaders(const std::string& logicShader, const std::string& renderShader);

		void addLogicShaderAttribute(GPUParticleSystem::ParticleAttribute attribute);
		void addRenderShaderAttribute(GPUParticleSystem::ParticleAttribute attribute);

		void startFrame();
		void addLogicShaderUniform(const std::string& name, ParticleUniform uniform);
		void addRenderShaderUniform(const std::string& name, ParticleUniform uniform);

	protected:

		//write objects as raw floats into a stream of data
		template<typename T>
		void writeData(float **ptr, const T& value)
		{
			const float* dataptr = (float *)&value;
			for (int i = 0; i < sizeof(T) / sizeof(float); i++)
			{
				**ptr = *dataptr++;
				(*ptr)++;
			}
		}

		void applyUniforms(demorender::Shader& shader, const UniformList& uniformList);

		std::vector<ParticleAttribute> m_logicShaderAttributes;
		std::vector<ParticleAttribute> m_renderShaderAttributes;

		UniformList m_logicUniforms;
		UniformList m_renderUniforms;

		int m_particleSize = 0;

		float* m_pInitialData = nullptr;
		std::string m_logicShader;
		std::string m_renderShader;

		std::string m_texture = "circle";

		int m_particleCount = 0;
		GLuint m_particleBuffer1 = GL_INVALID_VALUE;
		GLuint m_particleBuffer2 = GL_INVALID_VALUE;

		float m_time;
	};

}