#include "Mesh.h"
#include "MeshBuilder.h"
#include "Shader.h"
#include "Buffer.h"

using namespace demomath;

namespace
{
	bool EXTRA_DEBUG = false;
}

namespace demorender
{
	Mesh::Vertex::Vertex():
		position(0),
		uv(0),
		normal(0),
		color(1.f)
	{

	}

	Mesh::Face::Face() :
		v1(0),
		v2(0),
		v3(0),
		normal(0.f)
	{

	}

	Mesh::Mesh():
		m_VAO(-1),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_indexed(false),
		m_vertexCount(0),
		m_faceCount(0),
		m_streamFlags(StreamId::VERTEX_STREAM | StreamId::UV_STREAM | StreamId::NORMAL_STREAM)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::load(const std::string& name)
	{

	}

	void Mesh::setStreamFlags(unsigned int flags)
	{
		m_streamFlags = flags;
	}

	void Mesh::debugDraw()
	{
		GLint polygonModeBackup[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonModeBackup);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw(RenderMode::TRIANGLES);
		glPolygonMode(GL_FRONT_AND_BACK, polygonModeBackup[1]);

	}
	void Mesh::draw(RenderMode mode)
	{
		GLenum primitiveType = GL_TRIANGLES;
		int verticesPerPrimitive = 3;
		switch (mode)
		{
			case RenderMode::TRIANGLES: primitiveType = GL_TRIANGLES; break;
			case RenderMode::TRIANGLE_FAN: primitiveType = GL_TRIANGLE_FAN; break;
			case RenderMode::TRIANGLE_STRIP: primitiveType = GL_TRIANGLE_STRIP; break;
			case RenderMode::LINES: primitiveType = GL_LINES; verticesPerPrimitive = 2; break;
			case RenderMode::LINE_STRIP: primitiveType = GL_LINE_STRIP; break;
			case RenderMode::TRIANGLES_ADJACENCY: primitiveType = GL_TRIANGLES_ADJACENCY; break;

			default:
				g_debug << "Trying to render with invalid primitive type " << (int)mode << std::endl;


		};
		if (m_indexed)
		{
			glDrawElements(primitiveType, m_faceCount * verticesPerPrimitive, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(primitiveType, 0, m_vertexCount);
		}
	}

	int Mesh::getFaceCount()
	{
		return m_faceCount;
	}
	int Mesh::getVertexCount()
	{
		return m_vertexCount;
	}

	void Mesh::bind(Shader* shader)
	{
		glBindVertexArray(m_VAO); GL_DEBUG;
		m_vertexBuffer->bind();
		if (m_indexed)
		{
			m_indexBuffer->bind();
		}

		const size_t vertexSize = 12;// sizeof(Vertex);
		if (m_streamFlags & StreamId::VERTEX_STREAM)
		{
			glEnableVertexAttribArray(shader->attrib("vertexPosition")); GL_DEBUG;
			glVertexAttribPointer(shader->attrib("vertexPosition"), 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), NULL); GL_DEBUG;
		}

		if (m_streamFlags & UV_STREAM)
		{
			glEnableVertexAttribArray(shader->attrib("vertexTextureCoordinate")); GL_DEBUG;
			glVertexAttribPointer(shader->attrib("vertexTextureCoordinate"), 2, GL_FLOAT, GL_TRUE, vertexSize * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat))); GL_DEBUG;
		}

		if (m_streamFlags & NORMAL_STREAM)
		{
			glEnableVertexAttribArray(shader->attrib("vertexNormal")); GL_DEBUG;
			glVertexAttribPointer(shader->attrib("vertexNormal"), 3, GL_FLOAT, GL_TRUE, vertexSize * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat))); GL_DEBUG;
		}

		if (m_streamFlags & COLOR_STREAM)
		{
			glEnableVertexAttribArray(shader->attrib("vertexColor")); GL_DEBUG;
			glVertexAttribPointer(shader->attrib("vertexColor"), 4, GL_FLOAT, GL_TRUE, vertexSize * sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat))); GL_DEBUG;
		}
	}

	void Mesh::generateCube()
	{
		//generate VAO 
		glGenVertexArrays(1, &m_VAO); GL_DEBUG;
		glBindVertexArray(m_VAO); GL_DEBUG;

		//create VBO
		float cubeVertexData[] = {
			//  X     Y     Z       U     V, Nx, Ny, Nz, R, G, B, A
			// bottom
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f,

			// top
			-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,

			// front
			-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,

			// back
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.f, 0.f, -1.f, 1.f, 1.f, 1.f, 1.f,

			// left
			-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,

			// right
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f };

		m_vertexBuffer = std::make_unique<Buffer>();
		m_vertexBuffer->init(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

		m_indexed = false;
		m_faceCount = 12;
		m_vertexCount = 36;
	}

	void Mesh::generateSquare()
	{
		//generate VAO 
		glGenVertexArrays(1, &m_VAO); GL_DEBUG;
		glBindVertexArray(m_VAO); GL_DEBUG;

		//create VBO

		float squareVertexData[] = {
			//  X     Y     Z       U     V NX NY NZ, R, G, B, A

			// front
			0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,//left up
			1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,  //right up
			1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, //right down
			0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, //left up
			1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, //right down
			0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1 //left down
		};
		//upload raw mesh data to the VBO 
		m_vertexBuffer = std::make_unique<Buffer>();
		m_vertexBuffer->init(GL_ARRAY_BUFFER, sizeof(squareVertexData), squareVertexData, GL_STATIC_DRAW);

		m_indexed = false;
		m_faceCount = 2;
		m_vertexCount = 6;
	}

	void Mesh::generate(Usage usage, std::vector<Vertex>* vertices, std::vector<Face>* faces, bool calcNormals)
	{
		m_indexed = faces != nullptr;

		m_faces.clear();
		m_vertices.clear();
		//TODO: also clear opengl buffers

		if (EXTRA_DEBUG)
		{
			if (m_indexed)
				g_debug << "generating indexed mesh" << std::endl;
			else
				g_debug << "generating non-indexed mesh" << std::endl;
		}

		m_vertexCount = vertices->size();

		//copy vertices
		std::copy(vertices->begin(), vertices->end(), std::back_inserter(m_vertices));

		//copy faces and calc normals if applicable
		if (m_indexed)
		{
			m_faceCount = faces->size();

			std::copy(faces->begin(), faces->end(), std::back_inserter(m_faces));

			if (calcNormals)
				calculateNormals();
		}
		createBuffers(usage);
	}

	void Mesh::createBuffers(Usage usage)
	{
		m_usage = usage;
		//generate VAO 
		glGenVertexArrays(1, &m_VAO); GL_DEBUG;
		glBindVertexArray(m_VAO); GL_DEBUG;

		//create VBO
		int vertexBytes = m_vertexCount * sizeof(Vertex);
		float *vertexData = new float[vertexBytes];
		float *vertexPtr = vertexData;

		for (int i = 0; i < m_vertexCount; i++)
		{
			const Vertex &v = m_vertices[i];
			*vertexPtr++ = v.position.x;
			*vertexPtr++ = v.position.y;
			*vertexPtr++ = v.position.z;
			*vertexPtr++ = v.uv.x;
			*vertexPtr++ = v.uv.y;
			*vertexPtr++ = v.normal.x;
			*vertexPtr++ = v.normal.y;
			*vertexPtr++ = v.normal.z;
			*vertexPtr++ = v.color.r;
			*vertexPtr++ = v.color.g;
			*vertexPtr++ = v.color.b;
			*vertexPtr++ = v.color.a;
		}
		//upload raw mesh data to the VBO 
		//TODO: is it possible to just send m_pVertices? 

		GLenum bufferMode = GL_INVALID_ENUM;

		switch (m_usage)
		{
			case Usage::STATIC: bufferMode = GL_STATIC_DRAW; break;
			case Usage::DYNAMIC: bufferMode = GL_DYNAMIC_DRAW; break;
			case Usage::STREAM: bufferMode = GL_STREAM_DRAW; break;
		}

		m_vertexBuffer = std::make_unique<Buffer>();
		m_vertexBuffer->init(GL_ARRAY_BUFFER, vertexBytes, vertexData, bufferMode);

		delete[] vertexData;

		if (m_indexed)
		{
			//create IBO

			int faceBytes = m_faceCount * 3 * sizeof(int);// sizeof(Face);
			unsigned int *indexData = new unsigned int[m_faceCount * 3];
			unsigned int *indexPtr = indexData;

			for (int i = 0; i < m_faceCount; i++)
			{
				const Face& f = m_faces[i];
				*indexPtr++ = f.v1;
				*indexPtr++ = f.v2;
				*indexPtr++ = f.v3;
			}
			m_indexBuffer = std::make_unique<Buffer>();
			m_indexBuffer->init(GL_ELEMENT_ARRAY_BUFFER, faceBytes, indexData, bufferMode);

			delete[] indexData;
		}
		else
		{
			m_faceCount = m_vertexCount / 3;
		}
	}

	void Mesh::updateVertices(const void* data, size_t size)
	{
		m_vertexBuffer->fill(size, data);
	}

	void Mesh::calculateNormals()
	{
		if (!m_indexed)
		{
			g_debug << "trying to calculate normals for mesh without indices!" << std::endl;
			return;
		}
		for (int i = 0; i < m_vertexCount; i++)
		{
			m_vertices[i].normal = glm::vec3(0);
		}

		//calculate face normals
		for (int j = 0; j < m_faceCount; j++)
		{
			Face& f = m_faces[j];
			//TODO: experiment with removing normalize
			glm::vec3 v1 = glm::normalize(m_vertices[f.v3].position - m_vertices[f.v1].position);
			glm::vec3 v2 = glm::normalize(m_vertices[f.v2].position - m_vertices[f.v1].position);

			glm::vec3 n = glm::cross(v1, v2);
			f.normal = n;
			m_vertices[f.v1].normal += n;
			m_vertices[f.v2].normal += n;
			m_vertices[f.v3].normal += n;
		}

		for (int i = 0; i < m_vertexCount; i++)
		{
			m_vertices[i].normal = glm::normalize(m_vertices[i].normal);
		}
	}
}

