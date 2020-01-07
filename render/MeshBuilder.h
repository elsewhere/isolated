#pragma once

#include <string>
#include <vector>
#include <map>
#include "../glm/glm.hpp"

namespace demorender
{
	class MeshBuilder
	{
	public:
		MeshBuilder();
		~MeshBuilder();

		void start(bool indexed);
		void end();

		void addVertex(const glm::vec3& position);
		void addUv(const glm::vec2& uv);
		void addNormal(const glm::vec3& normal);
		void addColor(const glm::vec4& color);
		void addIndices(int v1, int v2, int v3);

		void addTriangleVertex(const glm::vec3& position1, const glm::vec3& position2, const glm::vec3& position3);
		void addTriangleUv(const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3);
		void addTriangleNormal(const glm::vec3& normal1, const glm::vec3& normal2, const glm::vec3& normal3);
		void addTriangleColor(const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3);
		void addTriangleIndices(int v1, int v2, int v3);

		class Mesh* getMesh();


		void generatePlane(int xres, int zres, float scale);

	private:
		bool m_indexed;
		std::vector<glm::vec3> m_positions;
		std::vector<glm::vec2> m_uvs;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec4> m_colors;
		std::vector<int> m_indices;
	};
}