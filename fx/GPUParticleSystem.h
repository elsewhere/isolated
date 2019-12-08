#pragma once

#include "../Globals.h"

namespace demofx
{
	class GPUParticleSystem
	{
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

	protected:

		std::vector<ParticleAttribute> m_logicShaderAttributes;
		std::vector<ParticleAttribute> m_renderShaderAttributes;
		int m_particleSize = 0;

		float* m_pInitialData = nullptr;
		std::string m_logicShader;
		std::string m_renderShader;
		int m_particleCount = 0;
		GLuint m_particleBuffer1 = GL_INVALID_VALUE;
		GLuint m_particleBuffer2 = GL_INVALID_VALUE;

		float m_time;
	};

}