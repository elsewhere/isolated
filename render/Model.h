#pragma once

#include <string>
#include "../glm/glm.hpp"
#include "Mesh.h"

namespace demorender
{

	class Model
	{
	public:
		Model();
		~Model();

		void debugDraw();
		void draw(Mesh::RenderMode mode = Mesh::TRIANGLES);
		void bind(class Shader* shader);
		void setMesh(const std::string& name);
		void setMesh(demorender::Mesh *pMesh);
		void setTransform(const glm::mat4& transform);
		glm::mat4 getTransform(); 
		demorender::Mesh* getMesh();

	private:
		glm::mat4 m_transform;
		class Mesh *m_pMesh;
	};
}