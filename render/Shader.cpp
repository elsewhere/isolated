#include "Shader.h"

using namespace democore;

namespace demorender
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Shader
	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLhandleARB Shader::sm_activeProgram = GL_INVALID_VALUE;

	Shader::Shader():
		m_program(-1),
		m_vertex(-1),
		m_fragment(-1),
		m_geometry(-1),
		m_compute(-1),
		m_tessControl(-1),
		m_tessEvaluation(-1),
		m_bound(false),
		m_hasBeenUsed(false),
		m_name("uninitialized")
	{
	}

	Shader::~Shader()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Init and bind
	//////////////////////////////////////////////////////////////////////////////////////////////////

	void Shader::init(const std::string& name, const ShaderInfo& info)
	{
		m_name = name;
		m_hasBeenUsed = false;		

		m_program = glCreateProgram(); GL_DEBUG;

		if (info.isCompute)
		{
			m_compute = glCreateShader(GL_COMPUTE_SHADER); GL_DEBUG;
			if (!m_compute)
			{
				g_debug << "could not create computer shader" << std::endl;
			}
			
			const char *computeSource = info.computeSource.c_str();
			glShaderSource(m_compute, 1, &computeSource, NULL); GL_DEBUG;
			glCompileShader(m_compute); GL_DEBUG;
			debug(m_compute, "output from vertex shader:");

			glAttachShader(m_program, m_compute);
		}
		else
		{
			m_vertex = glCreateShader(GL_VERTEX_SHADER); GL_DEBUG;
			if (!m_vertex)
				g_debug << "could not create vertex shader" << std::endl;

			const char *vertexSource = info.vertexSource.c_str();

			glShaderSource(m_vertex, 1, &vertexSource, NULL); GL_DEBUG;
			glCompileShader(m_vertex); GL_DEBUG;
			debug(m_vertex, "output from vertex shader:");

			if (info.hasGeometry)
			{
				m_geometry = glCreateShader(GL_GEOMETRY_SHADER); GL_DEBUG;
				if (!m_geometry)
				{
					g_debug << "could not create geometry shader" << std::endl;
				}
				else
				{
					const char* geometrySource = info.geometrySource.c_str();

					glShaderSource(m_geometry, 1, &geometrySource, NULL); GL_DEBUG;
					glCompileShader(m_geometry); GL_DEBUG;
					debug(m_geometry, "output from geometry shader:");
				}
			}

			if (info.hasFragment)
			{
				m_fragment = glCreateShader(GL_FRAGMENT_SHADER); GL_DEBUG;
				if (m_fragment == 0)
					g_debug << "could not create fragment shader" << std::endl;

				const char* fragmentSource = info.fragmentSource.c_str();

				glShaderSource(m_fragment, 1, &fragmentSource, NULL); GL_DEBUG;
				glCompileShader(m_fragment); GL_DEBUG;
				debug(m_fragment, "output from fragment shader:");
			}

			if (info.hasTessellation)
			{
				m_tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
				m_tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);

				if (m_tessControl == 0)
					g_debug << "could not create tess control shader" << std::endl;
				if (m_tessEvaluation == 0)
					g_debug << "could not create tess evaluation shader" << std::endl;

				const char* tessControlSource = info.tessControlSource.c_str();
				const char* tessEvaluationSource = info.tessEvaluationSource.c_str();

				glShaderSource(m_tessControl, 1, &tessControlSource, NULL); GL_DEBUG;
				glCompileShader(m_tessControl); GL_DEBUG;
				debug(m_tessControl, "output from tess control shader:");

				glShaderSource(m_tessEvaluation, 1, &tessEvaluationSource, NULL); GL_DEBUG;
				glCompileShader(m_tessEvaluation); GL_DEBUG;
				debug(m_tessEvaluation, "output from tess eval  shader:");

			}

			//there is always a vertex shader
			glAttachShader(m_program, m_vertex); GL_DEBUG;

			if (info.hasFeedbackVaryings)
			{
				std::vector<const char *> cstrings;
				for (auto& s : info.feedbackVaryings)
				{
					cstrings.push_back(s.c_str());
				}
				glTransformFeedbackVaryings(m_program, cstrings.size(), &cstrings[0], GL_INTERLEAVED_ATTRIBS);

			}
			if (info.hasGeometry)
			{
				glAttachShader(m_program, m_geometry); GL_DEBUG;
			}
			if (info.hasFragment)
			{
				glAttachShader(m_program, m_fragment); GL_DEBUG;
			}
			if (info.hasTessellation)
			{
				glAttachShader(m_program, m_tessControl);
				glAttachShader(m_program, m_tessEvaluation);
			}
		}

		glLinkProgram(m_program); GL_DEBUG;

		GLint status;
		glGetProgramiv(m_program, GL_LINK_STATUS, &status); GL_DEBUG;
		if (status == GL_FALSE)
		{
			g_debug << "could not link shader!" << std::endl;
			GLint infoLogLength;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetProgramInfoLog(m_program, infoLogLength, NULL, strInfoLog);
			g_debug << strInfoLog;

			std::stringstream ss;
			ss << "could not link shader " << m_name;
			g_error.log(ss.str());
			delete[] strInfoLog;
		}

	}

	void Shader::release()
	{
		glDeleteShader(m_vertex);
		glDeleteShader(m_fragment);
		glDeleteProgram(m_program);
	}

	void Shader::bind()
	{
		//avoid repeatedly binding the same program 
		if (sm_activeProgram != m_program)
		{
			m_hasBeenUsed = true;
			setBound(true);
			g_shaders->setBound(this);
			glUseProgram(m_program);
			RenderStatistics::sm_shadersBound++;

			sm_activeProgram = m_program;
		}
	}

	void Shader::debug(GLuint object, const std::string& type)
	{
		if (glIsShader(object))
		{
			const int MAX_LOG_LENGTH = 4096;
			char log[MAX_LOG_LENGTH];
			int logLength = 0;
			glGetShaderInfoLog(object, MAX_LOG_LENGTH, &logLength, log);
			if (logLength > 0)
			{
				g_debug << type << std::endl;
				g_debug << log << std::endl;
				
				std::stringstream ss(log);
				std::string to;

				while (std::getline(ss, to, '\n'))
				{
					if (!to.empty())
					{
						std::stringstream str;
						str << m_name << " - " << to;
						g_error.log(str.str());
					}
				}
			}
		}
		else
		{
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Parameter locations
	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLint Shader::uniform(const std::string& name)
	{
		if (m_uniformCache.find(name) == m_uniformCache.end())
		{
			GLint location = glGetUniformLocation(m_program, name.c_str());
			if (location == -1)
			{
				g_debug << "Cannot find uniform variable " << name << " in shader " << m_name << std::endl;
				return -1;
			}
//			g_debug << "could not find uniform variable " << name << ", location = " << 
//					location << ", inserting into cache" << std::endl;
			m_uniformCache[name] = location;
			return location;

		}
		else
		{
//			g_debug << "found cached uniform " << name << std::endl;
			return m_uniformCache[name];
		}
	}

	GLint Shader::attrib(const std::string& name)
	{
		if (m_attribCache.find(name) == m_attribCache.end())
		{
			GLint location = glGetAttribLocation(m_program, name.c_str());
			if (location == -1)
			{
				g_debug << "Cannot find attrib " << name << " in shader " << m_name << std::endl;
				return -1;
			}
//			g_debug << "could not find attrib " << name << ", location = " <<
//				location << ", inserting into cache" << std::endl;
			m_attribCache[name] = location;
			return location;

		}
		else
		{
//			g_debug << "found cached attribute " << name << std::endl;
			return m_attribCache[name];
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Uniform values
	//////////////////////////////////////////////////////////////////////////////////////////////////

	void Shader::debugBound()
	{
		if (!m_bound)
		{
			g_debug << "trying to set uniforms or vertex attributes but shader " << m_name << " is not bound" << std::endl;
		}
	}
	bool Shader::setUniform1f(const std::string& name, GLfloat v0)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform1f(location, v0);
		return true;
	}

	bool Shader::setUniform2f(const std::string& name, GLfloat v0, GLfloat v1) 
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform2f(location, v0, v1);

		return true;
	}
	bool Shader::setUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2) 
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform3f(location, v0, v1, v2);
		return true;
	}
	bool Shader::setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform4f(location, v0, v1, v2, v3);
		return true;
	}

	bool Shader::setUniform1i(const std::string& name, GLint v0)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform1i(location, v0);

		return true;
	}
	bool Shader::setUniform2i(const std::string& name, GLint v0, GLint v1)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform2i(location, v0, v1);
		return true;
	}
	bool Shader::setUniform3i(const std::string& name, GLint v0, GLint v1, GLint v2)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform3i(location, v0, v1, v2);
		return true;
	}
	bool Shader::setUniform4i(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform4i(location, v0, v1, v2, v3);
		return true;
	}

	bool Shader::setUniform1fv(const std::string& name, GLsizei count, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform1fv(location, count, value);

		return true;
	}
	bool Shader::setUniform2fv(const std::string& name, GLsizei count, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;
		glUniform2fv(location, count, value);

		return true;
	}
	bool Shader::setUniform3fv(const std::string& name, GLsizei count, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;
		glUniform3fv(location, count, value);

		return true;
	}
	bool Shader::setUniform4fv(const std::string& name, GLsizei count, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform4fv(location, count, value);
		return true;
	}
	bool Shader::setUniform1iv(const std::string& name, GLsizei count, GLint *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform1iv(location, count, value);

		return true;
	}
	bool Shader::setUniform2iv(const std::string& name, GLsizei count, GLint *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform2iv(location, count, value);
		return true;
	}
	bool Shader::setUniform3iv(const std::string& name, GLsizei count, GLint *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform3iv(location, count, value);
		return true;
	}
	bool Shader::setUniform4iv(const std::string& name, GLsizei count, GLint *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniform4iv(location, count, value);
		return true;
	}
	   
	bool Shader::setUniformMatrix2fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniformMatrix2fv(location, count, transpose, value);
		return true;
	}
	bool Shader::setUniformMatrix3fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniformMatrix3fv(location, count, transpose, value);
		return true;
	}
	bool Shader::setUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value)
	{
		debugBound();
		int location = uniform(name);
		if (location == -1)
			return false;

		glUniformMatrix4fv(location, count, transpose, value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Vertex attributes
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//vertex attributes
	bool Shader::setVertexAttrib1f(const std::string& name, GLfloat v0)
	{
		debugBound();
		int location = attrib(name);
		if (location == -1)
			return false;

		glVertexAttrib1f(location, v0);

		return true;
	}
	bool Shader::setVertexAttrib2f(const std::string& name, GLfloat v0, GLfloat v1)
	{
		debugBound();
		int location = attrib(name);
		if (location == -1)
			return false;

		glVertexAttrib2f(location, v0, v1);
		return true;
	}
	bool Shader::setVertexAttrib3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		debugBound();
		int location = attrib(name);
		if (location == -1)
			return false;
		glVertexAttrib3f(location, v0, v1, v2);

		return true;
	}
	bool Shader::setVertexAttrib4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		debugBound();
		int location = attrib(name);
		if (location == -1)
			return false;
		glVertexAttrib4f(location, v0, v1, v2, v3);

		return true;
	}
}