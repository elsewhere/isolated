#pragma once

#include "../Globals.h"


namespace demorender
{
	class ShaderInfo
	{
	public:
		ShaderInfo() {};
		~ShaderInfo() {};

		bool hasFragment = false;
		bool hasGeometry = false;
		bool hasFeedbackVaryings = false;
		bool hasTessellation = false;
		bool isCompute = false;

		std::string vertexSource;
		std::string fragmentSource;
		std::string geometrySource;
		std::string computeSource;
		std::string tessControlSource;
		std::string tessEvaluationSource;

		std::vector<std::string> feedbackVaryings;
	};

	class Shader
	{
	public:
		Shader();
		~Shader();

		//setup
		void init(const std::string& name, const ShaderInfo& info);
		void release();
		void bind();

		//parameter passing
		bool setUniform1f(const std::string& name, GLfloat v0);  
		bool setUniform2f(const std::string& name, GLfloat v0, GLfloat v1);
		bool setUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2); 
		bool setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); 
		
		bool setUniform1i(const std::string& name, GLint v0);
		bool setUniform2i(const std::string& name, GLint v0, GLint v1);
		bool setUniform3i(const std::string& name, GLint v0, GLint v1, GLint v2);
		bool setUniform4i(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3);
		
		bool setUniform1fv(const std::string& name, GLsizei count, GLfloat *value);
		bool setUniform2fv(const std::string& name, GLsizei count, GLfloat *value);
		bool setUniform3fv(const std::string& name, GLsizei count, GLfloat *value);
		bool setUniform4fv(const std::string& name, GLsizei count, GLfloat *value);
		bool setUniform1iv(const std::string& name, GLsizei count, GLint *value);
		bool setUniform2iv(const std::string& name, GLsizei count, GLint *value);
		bool setUniform3iv(const std::string& name, GLsizei count, GLint *value);
		bool setUniform4iv(const std::string& name, GLsizei count, GLint *value);
		   
		bool setUniformMatrix2fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value);
		bool setUniformMatrix3fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value);
		bool setUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, GLfloat *value);

		//vertex attributes
		bool setVertexAttrib1f(const std::string& name, GLfloat v0);
		bool setVertexAttrib2f(const std::string& name, GLfloat v0, GLfloat v1);
		bool setVertexAttrib3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
		bool setVertexAttrib4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);	

		bool hasBeenUsed() { return m_hasBeenUsed; };
		void setBound(bool bound) { m_bound = bound; };
		std::string getName() { return m_name; }

		GLint attrib(const std::string& name);
		GLint uniform(const std::string& name);
	private:

		std::vector<std::string> m_feedbackVaryings;
		std::map<const std::string, GLint> m_uniformCache;
		std::map<const std::string, GLint> m_attribCache;
		void debug(GLuint object, const std::string& type);
		void debugBound();

		bool m_bound;
		bool m_hasBeenUsed;
		std::string m_name;
		GLhandleARB m_vertex;
		GLhandleARB m_fragment;
		GLhandleARB m_geometry;
		GLhandleARB m_tessControl;
		GLhandleARB m_tessEvaluation;
		GLhandleARB m_compute;
		GLhandleARB m_program;


	};
}