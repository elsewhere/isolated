#include "MeshManager.h"
#include "MeshBuilder.h"
#include "../globals.h"
#include "../objloader/OBJ_Loader.h"

namespace demorender
{
	MeshManager* MeshManager::sm_instance = nullptr;

	MeshManager::MeshManager()
	{
	}
	MeshManager::~MeshManager()
	{
	}

	MeshManager* MeshManager::inst()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new MeshManager();
		}
		return sm_instance;
	}

	void MeshManager::init()
	{
		g_debug << "starting up mesh manager" << std::endl;
	}

	Mesh *MeshManager::getMesh(const std::string& name)
	{
		auto search = m_meshes.find(name);
		if (search != m_meshes.end())
		{
			return search->second;
		}
		else
		{
			g_debug << "could not find mesh " << name << std::endl;
			return 0;
		}
	}


	void MeshManager::addMesh(const std::string& name, Mesh *mesh)
	{
		m_meshes[name] = mesh;
	}

	void MeshManager::loadMeshes()
	{
		m_meshes.clear();
		//TODO: integrate assimp or something

		g_profiler.startProfile("Mesh loading");
		Mesh *cube = new Mesh();
		cube->generateCube();
		addMesh("cube", cube);

		Mesh *square = new Mesh();
		square->generateSquare();
		square->setStreamFlags(Mesh::VERTEX_STREAM | Mesh::UV_STREAM);
		addMesh("square", square);

		std::vector<std::string> filenames;
		std::string directory = "data\\meshes\\";
		StringUtils::iterateDirectory(directory, filenames);
		for (std::string& filename : filenames)
		{
			std::string path = directory + filename;

			g_debug << "loading mesh " << filename << std::endl;
			objl::Loader loader;
			bool success = loader.LoadFile(path);
			if (success)
			{
				MeshBuilder *builder = new MeshBuilder();
				builder->start(true);
				for (size_t i = 0; i < loader.LoadedMeshes.size(); i++)
				{
					objl::Mesh mesh = loader.LoadedMeshes[i];
					for (size_t j = 0; j < mesh.Vertices.size(); j++)
					{
						glm::vec3 pos = glm::vec3(mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y, mesh.Vertices[j].Position.Z);
						glm::vec3 nor = glm::vec3(mesh.Vertices[j].Normal.X, mesh.Vertices[j].Normal.Y, mesh.Vertices[j].Normal.Z);
						glm::vec2 uv = glm::vec2(mesh.Vertices[j].TextureCoordinate.X, mesh.Vertices[j].TextureCoordinate.Y);
						builder->addVertex(pos);
						builder->addNormal(nor);
						builder->addUv(uv);
					}
					for (size_t j = 0; j < mesh.Indices.size(); j += 3)
					{
						builder->addIndices(mesh.Indices[j], mesh.Indices[j + 1], mesh.Indices[j + 2]);
					}
				}

				std::string meshName = filename.substr(0, filename.length() - 4);
				Mesh *mesh = builder->getMesh();
				if (mesh)
				{
					addMesh(meshName, mesh);
				}
				else
				{
					g_debug << " - FAILED!!! ERROR WHEN CREATING MESH!!!" << std::endl;
				}

			}
			else
			{
				g_debug << " - FAILED! Could not load " << path << std::endl;
			}

		}
		g_profiler.endProfile("Mesh loading");
	}

}