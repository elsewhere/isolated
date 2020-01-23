#include "Buffer.h"

using namespace demomath;

namespace demorender
{
	int Buffer::sm_totalBufferSize = 0;
	int Buffer::sm_totalBufferCount = 0;

	int Buffer::sm_arrayBufferCount = 0;
	int Buffer::sm_ssboCount = 0;
	int Buffer::sm_indexBufferCount = 0;

	Buffer::Buffer():
		m_id(-1),
		m_size(0),
		m_type(GL_INVALID_ENUM)
	{
	}

	Buffer::~Buffer()
	{
	}

	int Buffer::getTotalBufferSize()
	{
		return sm_totalBufferSize;
	}

	int Buffer::getTotalBufferCount()
	{
		return sm_totalBufferCount;
	}

	void Buffer::init(GLenum type, int size, const void* initialData, GLenum usage)
	{
		m_type = type;
		m_size = size;

		if (m_size <= 0)
		{
			g_debug << "trying to create a buffer with zero or negative size" << std::endl;
			return;
		}
		
		if (usage == GL_INVALID_ENUM)
		{
			//set defaults
			switch (m_type)
			{
				case GL_ARRAY_BUFFER: usage = GL_STREAM_DRAW; break;
				case GL_SHADER_STORAGE_BUFFER: usage = GL_DYNAMIC_COPY; break;
				case GL_ELEMENT_ARRAY_BUFFER: usage = GL_STATIC_DRAW; break;
				default:
				{
					g_debug << "trying to init an unsupported buffer, sticking to defaults " << std::endl;
					usage = GL_DYNAMIC_COPY;
				}
			}
		}

		glGenBuffers(1, &m_id);
		glBindBuffer(m_type, m_id);
		glBufferData(m_type, size, initialData, usage);
		glBindBuffer(m_type, 0);

		sm_totalBufferSize += m_size;
		sm_totalBufferCount++;

		switch (type)
		{
			case GL_ARRAY_BUFFER: sm_arrayBufferCount++; break;
			case GL_SHADER_STORAGE_BUFFER: sm_ssboCount++; break;
			case GL_ELEMENT_ARRAY_BUFFER: sm_indexBufferCount++; break;
		}

	}

	void Buffer::fill(int dataSize, const void* data)
	{
		if (dataSize > m_size)
		{
			g_debug << "trying to write " << dataSize << " bytes of data into buffer that's " << m_size << " large, clipping data" << std::endl;
			dataSize = m_size;
		}
		if (m_type == GL_INVALID_ENUM)
		{
			g_debug << "trying to fill an uninitialized buffer" << std::endl;
			return;
		}


		glBindBuffer(m_type, m_id); GL_DEBUG;
		glBufferSubData(m_type, 0, dataSize, data); GL_DEBUG;
		glBindBuffer(m_type, 0); GL_DEBUG;
		/*

		glBindBuffer(m_type, m_id); GL_DEBUG;
		GLvoid* pBuffer = glMapBuffer(m_type, GL_WRITE_ONLY); GL_DEBUG;
		memcpy(pBuffer, data, dataSize); GL_DEBUG;
		glUnmapBuffer(m_type); GL_DEBUG;
		*/
	}
	
	void Buffer::bind()
	{
		glBindBuffer(m_type, m_id);
	}

	void Buffer::unbind()
	{
		glBindBuffer(m_type, 0);
	}

	GLuint Buffer::getID()
	{
		return m_id;
	}
	
	void Buffer::debugPrint()
	{
		g_debug << " " << std::endl;
		g_debug << "created " << sm_totalBufferCount << " OpenGL buffers, total size = " << sm_totalBufferSize << std::endl;
		g_debug << "  array buffers : " << sm_arrayBufferCount << std::endl;
		g_debug << "  shader storage buffers : " << sm_ssboCount << std::endl;
		g_debug << "  index buffers : " << sm_indexBufferCount << std::endl;
		g_debug << " " << std::endl;
	}
}