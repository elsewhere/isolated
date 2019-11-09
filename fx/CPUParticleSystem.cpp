#include "CPUParticleSystem.h"
#include "../glm/gtx/transform.hpp"

using namespace demorender;
using namespace democore;

namespace demofx
{
	CPUParticleSystem::CPUParticleSystem(const std::string& shader, const std::string& texture, int particleCount):
		m_particleCount(particleCount),
		m_pParticleData(nullptr),
		m_currentBuffer(0),
		m_shader(shader),
		m_texture(texture)
	{
		m_pParticleData = new ParticleData[m_particleCount];

		glGenVertexArrays(2, &m_vao[0]); GL_DEBUG;
		glGenBuffers(2, &m_vbo[0]); GL_DEBUG;

		for (int i = 0; i < 2; i++)
		{
			glBindVertexArray(m_vao[i]); GL_DEBUG;
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[i]); GL_DEBUG;
			
			glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleData) * m_particleCount, &m_pParticleData[0], GL_DYNAMIC_DRAW); GL_DEBUG;

			//vertices
			glEnableVertexAttribArray(0); GL_DEBUG;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, position))); GL_DEBUG;

			//size
			glEnableVertexAttribArray(1); GL_DEBUG;
			glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, size))); GL_DEBUG;

			//color
			glEnableVertexAttribArray(2); GL_DEBUG;
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, color))); GL_DEBUG;

			//energy
			glEnableVertexAttribArray(3); GL_DEBUG;
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, energy))); GL_DEBUG;

			//max energy
			glEnableVertexAttribArray(4); GL_DEBUG;
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, maxEnergy))); GL_DEBUG;

		}
	}

	void CPUParticleSystem::setInitialValues()
	{
		for (int i = 0; i < m_particleCount; i++)
		{
			m_pParticleData[i].position = Math::randVectSphere() * 15.f;
			m_pParticleData[i].direction = Math::randVectSphere();
			m_pParticleData[i].size = Math::randBetween(0.02f, 0.4f);
			m_pParticleData[i].color = glm::vec4(1.f, 1.f, 1.f, Math::randBetween(0.1f, 0.5f));
			m_pParticleData[i].energy = 1.f;
			m_pParticleData[i].maxEnergy = 1.f;
		}
	}

	void CPUParticleSystem::setShader(const std::string& shader)
	{
		m_shader = shader;
	}
	void CPUParticleSystem::setTexture(const std::string& texture)
	{
		m_texture = texture;
	}

	demorender::Shader& CPUParticleSystem::getShader()
	{
		g_shaders->getShader(m_shader).bind();
		return g_shaders->getShader(m_shader);
	}

	void CPUParticleSystem::draw(demorender::Camera* pCamera)
	{
		Shader& s = g_shaders->getShader(m_shader);
		s.bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glDisable(GL_DEPTH_TEST);
		glm::mat4 modelMatrix = glm::mat4(1.f);

		g_textures->bindTexture(m_texture, GL_TEXTURE0);
		s.setUniform1i("tex", 0);
		s.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, (float *)&pCamera->getViewMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, (float *)&pCamera->getProjectionMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;

		glBindVertexArray(m_vao[(m_currentBuffer + 1) % 2]); GL_DEBUG;
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[(m_currentBuffer + 1) % 2]); GL_DEBUG;

		glEnableVertexAttribArray(0); GL_DEBUG;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, position))); GL_DEBUG;

		glEnableVertexAttribArray(1); GL_DEBUG;
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, size))); GL_DEBUG;

		glEnableVertexAttribArray(2); GL_DEBUG;
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleData), reinterpret_cast<void *>(offsetof(ParticleData, color))); GL_DEBUG;

		glDrawArrays(GL_POINTS, 0, m_particleCount); GL_DEBUG;
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void CPUParticleSystem::update()
	{
		//logic

		const float speed = 0.01f;
		for (int i = 0; i < m_particleCount; i++)
		{
			m_pParticleData[i].position += m_pParticleData[i].direction * speed;
		}
		updateBuffers();
	}


	void CPUParticleSystem::updateBuffers()
	{
		// bind a buffer to upload to and invalidate data
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[m_currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleData) * m_particleCount, 0, GL_DYNAMIC_DRAW);

		ParticleData* writePtr = reinterpret_cast<ParticleData*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ParticleData) * m_particleCount, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)); GL_DEBUG;

		if (writePtr == 0)
		{
			g_debug << "writeptr null in particle system!" << std::endl;
		}
		else
		{
			memcpy(writePtr, m_pParticleData, m_particleCount * sizeof(ParticleData));
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		m_currentBuffer = (m_currentBuffer + 1) % 2;
	}

	CPUParticleSystem::~CPUParticleSystem()
	{
		delete[] m_pParticleData;
	}

}