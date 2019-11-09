#pragma once

#include <string>
#include <vector>
#include <map>

#define g_meshes MeshManager::inst()

namespace demorender
{
	class MeshManager
	{
	public:
		void init();
		void clear();
		void loadMeshes();

		static MeshManager* inst();

		//textures
		void addMesh(const std::string& name, class Mesh* mesh);

		Mesh *getMesh(const std::string& name);

	private:
		MeshManager();
		~MeshManager();


		static const int MAX_TEXTURES = 8;
		std::map<std::string, class Mesh*> m_meshes;

		static MeshManager* sm_instance;

	};
}