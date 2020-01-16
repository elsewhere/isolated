#pragma once

#include <string>
#include <vector>
#include "../glm/glm.hpp"

namespace demorender
{
	class Mesh
	{
	public:

		enum StreamId
		{
			VERTEX_STREAM = (1 << 1),
			UV_STREAM = (1 << 2),
			NORMAL_STREAM = (1 << 3),
			COLOR_STREAM = (1 << 4)
		};

		enum class RenderMode
		{
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
			LINES,
			LINE_STRIP,
			TRIANGLES_ADJACENCY,
			LINES_ADJACENCY
		};

		enum class Usage
		{
			STATIC,
			DYNAMIC,
			STREAM

		};

		struct Vertex
		{
			Vertex();

			glm::vec3 position;
			glm::vec2 uv;
			glm::vec3 normal;
			glm::vec4 color;
		};

		struct Face
		{
			Face();

			int v1, v2, v3;
			glm::vec3 normal;
		};

		Mesh();
		~Mesh();

		void load(const std::string& name);
		void generateCube();
		void generateSquare();
		void generate(Usage usage, std::vector<Vertex>* vertices, std::vector<Face>* faces = nullptr, bool calcNormals = true);

		void updateVertices(const void* data, size_t size);
		int getVertexCount();
		int getFaceCount();

		void setStreamFlags(unsigned int streams);
		void bind(class Shader* shader);

		void draw(RenderMode mode = RenderMode::TRIANGLES);
		void debugDraw();

		void calculateNormals();
		void createBuffers(Usage usage);

	private:
		unsigned int m_streamFlags;
		int m_faceCount;
		int m_vertexCount;
		bool m_indexed;
		unsigned int m_VAO;

		Usage m_usage;

		Face* m_pFaces;
		Vertex* m_pVertices;
		std::unique_ptr<class Buffer> m_vertexBuffer;
		std::unique_ptr<class Buffer> m_indexBuffer;
	};
}