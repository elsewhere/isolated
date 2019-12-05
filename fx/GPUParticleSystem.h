#pragma once

#include "../Globals.h"

namespace demofx
{
	class GPUParticleSystem
	{

	public:
		GPUParticleSystem(int particleCount);
		~GPUParticleSystem();

		virtual void setInitialData();
		void createBuffers();
		void update();
		void draw(demorender::Camera *pCamera);
		void setShader(const std::string& shader);

	protected:
#pragma pack(push, 1)
		struct Particle
		{
			glm::vec3 position;
			glm::vec3 direction;
			glm::vec4 color;
			float energy;
			float maxEnergy;
		};
#pragma pack(pop)
		Particle* m_pInitialData;
		std::string m_shader;
		int m_particleCount;
		GLuint m_particleBuffer1;
		GLuint m_particleBuffer2;

		float m_time;
	};

}