#pragma once

#include "../Globals.h"

namespace demorender
{
	class Buffer
	{
	public:

		Buffer();
		~Buffer();

		void init(GLenum type, int size, const void* initialData, GLenum usage = GL_INVALID_ENUM);
		void fill(int dataSize, const void* data);
		void bind();
		void unbind();

		static int getTotalBufferSize();
		static int getTotalBufferCount();
		
		static void debugPrint();

		GLuint getID();
	private:
		static int sm_totalBufferSize;
		static int sm_totalBufferCount;
		static int sm_arrayBufferCount;
		static int sm_ssboCount;
		static int sm_indexBufferCount;

		GLenum m_type;
		int m_size;
		GLuint m_id;
	};
}