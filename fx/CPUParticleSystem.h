#pragma once

#include "../Globals.h"

namespace demofx
{
	class CPUParticleSystem
	{

	public:
		CPUParticleSystem(const std::string& shader, const std::string& texture, int particleCount);
		~CPUParticleSystem();

		void setShader(const std::string& shader);
		void setTexture(const std::string& texture);
		virtual void update();
		virtual void draw(demorender::Camera* pCamera);
		virtual void setInitialValues();

		demorender::Shader& getShader();

	protected:
#pragma pack(push, 1)
		struct ParticleData
		{
			glm::vec3 position;
			glm::vec3 direction;
			glm::vec4 color;
			
			float energy;
			float maxEnergy;
			float size;
		};
#pragma pack(pop)

		void updateBuffers();
		std::string m_shader;
		std::string m_texture;

		unsigned int m_vao[2];
		unsigned int m_vbo[2];

		int m_currentBuffer;

		int m_particleCount;

		ParticleData* m_pParticleData;
	};

}