#include "GPUParticleSystem.h"
#include "../glm/gtx/transform.hpp"

namespace
{
	bool EXTRA_DEBUG = true;
}

using namespace demorender;
using namespace democore;

namespace demofx
{
	GPUParticleSystem::GPUParticleSystem(int particleCount):
		m_particleCount(particleCount),
		m_particleBuffer1(-1),
		m_particleBuffer2(-1),
		m_pInitialData(nullptr),
		m_time(0.f)
	{

	}

	GPUParticleSystem::~GPUParticleSystem()
	{

	}

	void GPUParticleSystem::setInitialData()
	{
		if (m_particleCount <= 0)
		{
			g_error.log("less than zero particles requested");
			return;
		}
		m_pInitialData = new Particle[m_particleCount];

		for (int i = 0; i < m_particleCount; i++)
		{
			Particle& p = m_pInitialData[i];
			p.direction = Math::randVectSphere();
			p.position = Math::randVectSphere();
			p.color = glm::vec4(1.f);
			p.energy = p.maxEnergy = Math::randFloat(0.5, 1.5f);
		}
	}

	void GPUParticleSystem::createBuffers()
	{
		if (m_particleCount <= 0)
		{
			g_error.log("less than zero particles requested");
			return;
		}

		const size_t dataSize = sizeof(Particle) * m_particleCount;
		if (EXTRA_DEBUG)
		{
			g_debug << "creating gpu particle buffer with " << m_particleCount << " particles, datasize = " << dataSize << std::endl;
		}


		//buffers		
		glGenBuffers(1, &m_particleBuffer1);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer1);
		glBufferData(GL_ARRAY_BUFFER, dataSize, m_pInitialData ? &m_pInitialData[0] : 0, GL_STREAM_DRAW);
		if (glGetError() == GL_OUT_OF_MEMORY)
		{
			g_error.log("out of GL memory while trying to create particle buffer");
			return;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_particleBuffer2);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer2);
		glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_STREAM_DRAW);
		if (glGetError() == GL_OUT_OF_MEMORY)
		{
			g_error.log("out of GL memory while trying to create particle buffer");
			return;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (EXTRA_DEBUG)
		{
			g_debug << "buffer1 = " << m_particleBuffer1 << " buffer2 = " << m_particleBuffer2 << std::endl;
		}
		delete[] m_pInitialData;
		m_pInitialData = nullptr;
	}
	void GPUParticleSystem::setShader(const std::string& shader)
	{
		m_shader = shader;
	}

	void GPUParticleSystem::update()
	{
		Shader& s = g_shaders->getShader(m_shader);
		s.bind();

		m_time += 0.01f; 
		s.setUniform1f("time", m_time);

		GL_DEBUG;
		glEnable(GL_RASTERIZER_DISCARD);
		GL_DEBUG;
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer1);

		glEnableVertexAttribArray(s.attrib("particlePosition"));
		GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("particleDirection"));
		GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("particleColor"));
		GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("particleEnergy"));
		GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("particleMaxEnergy"));
		GL_DEBUG;

		glVertexAttribPointer(s.attrib("particlePosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, position));
		GL_DEBUG;
		glVertexAttribPointer(s.attrib("particleDirection"), 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, direction));
		GL_DEBUG;
		glVertexAttribPointer(s.attrib("particleColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, color));
		GL_DEBUG;
		glVertexAttribPointer(s.attrib("particleEnergy"), 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, energy));
		GL_DEBUG;
		glVertexAttribPointer(s.attrib("particleMaxEnergy"), 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, maxEnergy));
		GL_DEBUG;

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer2);
		GL_DEBUG;

		glBeginTransformFeedback(GL_POINTS);
		GL_DEBUG;
		//tekstuuri tähän jos tarvitaan
		glDrawArrays(GL_POINTS, 0, m_particleCount); //do it 
		GL_DEBUG;
		glEndTransformFeedback();
		GL_DEBUG;
		glDisable(GL_RASTERIZER_DISCARD);
		GL_DEBUG;

		std::swap(m_particleBuffer1, m_particleBuffer2);
	}

	void GPUParticleSystem::draw(Camera *pCamera)
	{
		//draw the contents of particlebuffer1
		Shader& s = g_shaders->getShader("gpuparticlesimple");

		s.bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glDisable(GL_DEPTH_TEST);
		glm::mat4 modelMatrix = glm::mat4(1.f);

		g_textures->bindTexture("circle", GL_TEXTURE0);
		s.setUniform1i("tex", 0);
		s.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, (float *)&pCamera->getViewMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, (float *)&pCamera->getProjectionMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;

		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer1); GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("vertexPosition")); GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("vertexColor")); GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("vertexEnergy")); GL_DEBUG;
		glEnableVertexAttribArray(s.attrib("vertexMaxEnergy")); GL_DEBUG;

		glVertexAttribPointer(s.attrib("vertexPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, position)); GL_DEBUG;
		glVertexAttribPointer(s.attrib("vertexColor"), 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, color)); GL_DEBUG;
		glVertexAttribPointer(s.attrib("vertexEnergy"), 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, energy)); GL_DEBUG;
		glVertexAttribPointer(s.attrib("vertexMaxEnergy"), 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)offsetof(Particle, maxEnergy)); GL_DEBUG;

		glDrawArrays(GL_POINTS, 0, m_particleCount); GL_DEBUG;

		glDisableVertexAttribArray(s.attrib("vertexPosition")); GL_DEBUG;
		glDisableVertexAttribArray(s.attrib("vertexColor")); GL_DEBUG;
		glDisableVertexAttribArray(s.attrib("vertexEnergy")); GL_DEBUG;
		glDisableVertexAttribArray(s.attrib("vertexMaxEnergy")); GL_DEBUG;

		g_shaders->unbindShader();
		glDisable(GL_BLEND);

	}
}