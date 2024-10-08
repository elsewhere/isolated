#include "MeshBuilder.h"
#include "../Globals.h"

namespace demorender
{
	MeshBuilder::MeshBuilder():
		m_indexed(false)
	{

	}

	MeshBuilder::~MeshBuilder()
	{

	}

	void MeshBuilder::start(bool indexed)
	{
		m_indexed = indexed;

		m_colors.clear();
		m_positions.clear();
		m_uvs.clear();
		m_indices.clear();
		m_normals.clear();
	}

	void MeshBuilder::end()
	{

	}

	void MeshBuilder::addVertex(const glm::vec3& position)
	{
		m_positions.push_back(position);
	}
	void MeshBuilder::addUv(const glm::vec2& uv)
	{
		m_uvs.push_back(uv);
	}
	void MeshBuilder::addNormal(const glm::vec3& normal)
	{
		m_normals.push_back(normal);

	}
	void MeshBuilder::addColor(const glm::vec4& color)
	{
		m_colors.push_back(color);
	}

	void MeshBuilder::addTriangleVertex(const glm::vec3& position1, const glm::vec3& position2, const glm::vec3& position3)
	{
		addVertex(position1);
		addVertex(position2);
		addVertex(position3);

	}
	void MeshBuilder::addTriangleUv(const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
	{
		addUv(uv1);
		addUv(uv2);
		addUv(uv3);

	}
	void MeshBuilder::addTriangleNormal(const glm::vec3& normal1, const glm::vec3& normal2, const glm::vec3& normal3)
	{
		addNormal(normal1);
		addNormal(normal2);
		addNormal(normal3);

	}
	void MeshBuilder::addTriangleColor(const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3)
	{
		addColor(color1);
		addColor(color2);
		addColor(color3);
	}

	void MeshBuilder::addIndices(int v1, int v2, int v3)
	{
		if (m_indexed)
		{
			m_indices.push_back(v1);
			m_indices.push_back(v2);
			m_indices.push_back(v3);
		}
		else
		{
			g_debug << "MeshBuilder - tryign to add indices to a mesh that is not indexed " << std::endl;
		}
	}

	Mesh* MeshBuilder::getMesh(demorender::Mesh::Usage usage)
	{
		int positionCount = m_positions.size();
		int uvCount = m_uvs.size();
		int normalCount = m_normals.size();
		int colorCount = m_colors.size(); 

		bool hasNormals = m_normals.size() > 0;
		bool hasColors = m_colors.size() > 0;
		bool hasUvs = m_uvs.size() > 0;

		if ((hasUvs && (positionCount != uvCount)) || (hasNormals && (positionCount != normalCount)) || 
			(hasColors && (positionCount != colorCount)))
		{
			g_debug << "MeshBuilder getMesh - invalid number of vertices! positions = " <<
				positionCount << " uvs = " << uvCount << " normals = " << normalCount << " colors = " 
				<< colorCount << std::endl;
			return nullptr;
		}

		std::vector<Mesh::Vertex> vertices;
		std::vector<Mesh::Face> faces;
		for (int i = 0; i < positionCount; i++)
		{
			Mesh::Vertex v;
			v.position = m_positions.at(i);
			if (hasUvs)
			{
				v.uv = m_uvs.at(i);
			}
			else
			{
				v.uv = glm::vec2(0.f);
			}

			if (hasNormals)
			{
				v.normal = m_normals.at(i);
			}
			else
			{
				v.normal = glm::vec3(0.f);
			}
			if (hasColors)
			{
				v.color = m_colors.at(i);
			}
			else
			{
				v.color = glm::vec4(1.f);
			}
			vertices.push_back(v);
		}

		if (m_indexed)
		{
			int indexCount = m_indices.size();
			int triangleCount = indexCount / 3;
			if (indexCount == 0)
			{
				g_debug << "Trying to create an indexed mesh with no indices" << std::endl;
				return nullptr;
			}
			for (int i = 0; i < triangleCount; i++)
			{
				int offset = i * 3;
				Mesh::Face f;
				f.v1 = m_indices.at(offset + 0);
				f.v2 = m_indices.at(offset + 1);
				f.v3 = m_indices.at(offset + 2);

				if (f.v1 >= positionCount || f.v2 >= positionCount || f.v3 >= positionCount)
				{
					g_debug << "Index too large (" << f.v1 << "," << f.v2 << "," << f.v3 << ")" << std::endl;
					return nullptr;
				}
				if (f.v1 < 0 || f.v2 < 0 || f.v3 < 0)
				{
					g_debug << "Index negative (" << f.v1 << "," << f.v2 << "," << f.v3 << ")" << std::endl;
					return nullptr;
				}
				faces.push_back(f);
			}
		}
		
		g_debug << "MeshBuilder getMesh - creating mesh with " << vertices.size() << " vertices" << std::endl;
		Mesh *mesh = new Mesh();
		mesh->generate(usage, &vertices, m_indexed ? &faces : 0, true);
		mesh->setStreamFlags(Mesh::VERTEX_STREAM | (hasUvs ? Mesh::UV_STREAM : 0) | (hasNormals ? Mesh::NORMAL_STREAM : 0));
		return mesh;
	}

	void MeshBuilder::generateCube(float scale)
	{
		glm::vec3 v0 = glm::vec3(-1, -1, -1) * scale;
		glm::vec3 v1 = glm::vec3( 1, -1, -1) * scale;
		glm::vec3 v2 = glm::vec3( 1,  1, -1) * scale;
		glm::vec3 v3 = glm::vec3(-1,  1, -1) * scale;

		glm::vec3 v4 = glm::vec3(-1, -1, 1) * scale;
		glm::vec3 v5 = glm::vec3(1, -1, 1) * scale;
		glm::vec3 v6 = glm::vec3(1, 1, 1) * scale;
		glm::vec3 v7 = glm::vec3(-1, 1, 1) * scale;

		/*      
				v0__________ v1
		         /|        /|
		        / |       / |
		       /  |      /  |
		    v3/___|_____/v2 |
			  | v4|_____|___| v5
			  |  /      |  /
			  | /       | / 
			  |/        |/  
			v7/_________/ v6

		*/

		start(false);

		//top side
		addTriangleVertex(v0, v1, v2);
		addTriangleVertex(v0, v2, v3);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		//right side
		addTriangleVertex(v2, v1, v5);
		addTriangleVertex(v2, v5, v6);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		//left side		
		addTriangleVertex(v0, v3, v7);
		addTriangleVertex(v0, v7, v4);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		//front side
		addTriangleVertex(v3, v2, v6);
		addTriangleVertex(v3, v6, v7);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		//back side
		addTriangleVertex(v1, v0, v4);
		addTriangleVertex(v1, v4, v5);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		//bottom side
		addTriangleVertex(v7, v6, v5);
		addTriangleVertex(v7, v5, v4);
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f));
		addTriangleUv(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec2(0.f, 1.f));

		end();

	}

	void MeshBuilder::generatePlane(int xres, int zres, float scale)
	{
/*
		0    1    2    3    4    5    6    7
		x----x----x----x----x----x----x----x
		|  / |  / |  / |  / |  / |  / |  / |
		| /  | /  | /  | /  | /  | /  | /  |
		x----x----x----x----x----x----x----x
		|  / |  / |  / |  / |  / |  / |  / |
		| /  | /  | /  | /  | /  | /  | /  |
		x----x----x----x----x----x----x----x
		|  / |  / |  / |  / |  / |  / |  / |
		| /  | /  | /  | /  | /  | /  | /  |
		x----x----x----x----x----x----x----x
		|  / |  / |  / |  / |  / |  / |  / |
		| /  | /  | /  | /  | /  | /  | /  |
		x----x----x----x----x----x----x----x
*/
		start(false);
		for (int z = 0; z < zres; z++)
		{
			float zt = (float)z / (float)zres;
			float zt2 = (zt - 0.5f) * 2.f;
			const float zstep = (1.f / zres) * 2.f;
			for (int x = 0; x < xres; x++)
			{
				float xt = (float)x / (float)xres;
				// [-1, 1]
				float xt2 = (xt - 0.5f) * 2.f;
				const float xstep = (1.f / xres) * 2.f;
				/*
				1----2
				|  / |
				| /  |
				4----3
				*/

				glm::vec3 vertex1 = glm::vec3(xt2, 0.f, zt2) * scale;
				glm::vec3 vertex2 = glm::vec3(xt2 + xstep, 0.f, zt2) * scale;
				glm::vec3 vertex3 = glm::vec3(xt2 + xstep, 0.f, zt2 + zstep) * scale;
				glm::vec3 vertex4 = glm::vec3(xt2, 0.f, zt2 + zstep) * scale;

				glm::vec2 uv1 = glm::vec2(xt, zt);
				glm::vec2 uv2 = glm::vec2(xt + xstep * 0.5f, zt);
				glm::vec2 uv3 = glm::vec2(xt + xstep * 0.5f, zt + zstep * 0.5f);
				glm::vec2 uv4 = glm::vec2(xt, zt + zstep * 0.5f);

				addVertex(vertex1);
				addUv(uv1);
				addVertex(vertex2);
				addUv(uv2);
				addVertex(vertex4);
				addUv(uv4);
				addVertex(vertex2);
				addUv(uv2);
				addVertex(vertex3);
				addUv(uv3);
				addVertex(vertex4);
				addUv(uv4);
			}
		}
		end();
	}
}