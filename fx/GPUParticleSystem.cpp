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

	void GPUParticleSystem::addLogicShaderAttribute(GPUParticleSystem::ParticleAttribute attribute)
	{
		if (std::find(std::begin(m_logicShaderAttributes), std::end(m_logicShaderAttributes), attribute) != m_logicShaderAttributes.end())
		{
			g_debug << "Trying to add logic shader attribute " << attribute.name << " to particle system twice\n";
		}
		else
		{
			m_particleSize += attribute.size;
			m_logicShaderAttributes.push_back(attribute);
			g_debug << "Added logic shader attribute " << attribute.name << " size = " << attribute.size << " total size = " << m_particleSize << "\n";
		}
	}
	void GPUParticleSystem::addRenderShaderAttribute(GPUParticleSystem::ParticleAttribute attribute)
	{
		if (std::find(std::begin(m_logicShaderAttributes), std::end(m_logicShaderAttributes), attribute) != m_logicShaderAttributes.end())
		{
			g_debug << "Trying to add render shader attribute " << attribute.name << " to particle system twice\n";
		}
		else
		{
			g_debug << "Added render shader attribute " << attribute.name << " size = " << attribute.size << "\n";
			m_renderShaderAttributes.push_back(attribute);
		}
	}


	void GPUParticleSystem::startFrame()
	{
		m_logicUniforms.clear();
		m_renderUniforms.clear();
	}

	void GPUParticleSystem::addLogicShaderUniform(const std::string& name, ParticleUniform uniform)
	{
		m_logicUniforms.push_back(std::make_pair(name, uniform));
	}
	void GPUParticleSystem::addRenderShaderUniform(const std::string& name, ParticleUniform uniform)
	{
		m_renderUniforms.push_back(std::make_pair(name, uniform));
	}

	void GPUParticleSystem::setInitialData()
	{
		if (m_particleCount <= 0)
		{
			g_error.log("less than zero particles requested");
			return;
		}
		if (m_particleSize <= 0)
		{
			g_error.log("no particle attributes have been set");
			return;
		}

		m_pInitialData = new float[m_particleCount * m_particleSize];
		memset(m_pInitialData, m_particleCount, m_particleSize * sizeof(float));
	}

	void GPUParticleSystem::createBuffers()
	{
		if (m_particleCount <= 0)
		{
			g_error.log("less than zero particles requested");
			return;
		}

		const size_t dataSize = m_particleSize * m_particleCount;
		if (EXTRA_DEBUG)
		{
			g_debug << "creating gpu particle buffer with " << m_particleCount << " particles, particle size = " << m_particleSize << " total = " << dataSize << std::endl;
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
	void GPUParticleSystem::setShaders(const std::string& logicShader, const std::string& renderShader)
	{
		m_logicShader = logicShader;
		m_renderShader = renderShader;
	}

	void GPUParticleSystem::applyUniforms(demorender::Shader& shader, const UniformList& uniformList)
	{
		for (const auto &u : uniformList)
		{
			if (std::holds_alternative<int>(u.second))
				shader.setUniform1i(u.first, std::get<int>(u.second));
			else if (std::holds_alternative<float>(u.second))
				shader.setUniform1f(u.first, std::get<float>(u.second));
			else if (std::holds_alternative<glm::vec3>(u.second))
				shader.setUniform3fv(u.first, 1, (float *)&(std::get<glm::vec3>(u.second)));
			else if (std::holds_alternative<glm::vec4>(u.second))
				shader.setUniform4fv(u.first, 1, (float *)&(std::get<glm::vec4>(u.second)));
			else if (std::holds_alternative<glm::mat4>(u.second))
				shader.setUniformMatrix4fv(u.first, 1, GL_FALSE, (float *)&(std::get<glm::mat4>(u.second)));
			else
			{
				g_debug << "ERROR trying to apply uniforms to shader " << shader.getName() << " in GPUParticleSystem\n";
			}
		}
	}

	void GPUParticleSystem::update()
	{
		Shader& s = g_shaders->getShader(m_logicShader);
		s.bind();

		m_time += 0.01f; 

		applyUniforms(s, m_logicUniforms);

		g_textures->bindTexture(m_logicTexture, GL_TEXTURE0);

		s.setUniform1f("time", m_time);

		GL_DEBUG;
		glEnable(GL_RASTERIZER_DISCARD);
		GL_DEBUG;
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer1);

		int offset = 0;
		for (const auto &a : m_logicShaderAttributes)
		{
			glEnableVertexAttribArray(s.attrib(a.name));  GL_DEBUG;
			glVertexAttribPointer(s.attrib(a.name), a.size, GL_FLOAT, GL_FALSE, m_particleSize * sizeof(float), (void *)(offset * sizeof(float))); GL_DEBUG;
			offset += a.size;
		}

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer2);
		GL_DEBUG;

		glBeginTransformFeedback(GL_POINTS);
		GL_DEBUG;
		//tekstuuri tähän jos tarvitaan
		glDrawArrays(GL_POINTS, 0, m_particleCount);
		GL_DEBUG;
		glEndTransformFeedback();
		GL_DEBUG;
		glDisable(GL_RASTERIZER_DISCARD);
		GL_DEBUG;

		std::swap(m_particleBuffer1, m_particleBuffer2);
	}

	void GPUParticleSystem::draw(Camera *pCamera, int numParticles)
	{
		//draw the contents of particlebuffer1
		Shader& s = g_shaders->getShader(m_renderShader);
		s.bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glDisable(GL_DEPTH_TEST);
		g_textures->bindTexture(m_renderTexture, GL_TEXTURE0);

		applyUniforms(s, m_renderUniforms);

		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer1); GL_DEBUG;
		int offset = 0;
		for (const auto& a : m_renderShaderAttributes)
		{
			glEnableVertexAttribArray(s.attrib(a.name)); GL_DEBUG;
			glVertexAttribPointer(s.attrib(a.name), a.size, GL_FLOAT, GL_FALSE, m_particleSize * sizeof(float), (void *)(offset * sizeof(float))); GL_DEBUG;
			offset += a.size;
		}

		glDrawArrays(GL_POINTS, 0, numParticles == -1 ? m_particleCount : numParticles); GL_DEBUG;

		for (const auto& a : m_renderShaderAttributes)
		{
			glDisableVertexAttribArray(s.attrib(a.name));
			GL_DEBUG;
		}

		g_shaders->unbindShader();
		glDisable(GL_BLEND);

	}
}