#include "Model.h"
#include "Mesh.h"
#include "Shader.h"

using namespace demomath;

namespace demorender
{
	Model::Model():
		m_transform(glm::mat4(1.f)),
		m_pMesh(0)
	{
	}

	Model::~Model()
	{
	}

	void Model::debugDraw()
	{
		m_pMesh->debugDraw();
	}
	void Model::draw(Mesh::RenderMode mode)
	{
		m_pMesh->draw(mode);
	}

	void Model::setMesh(const std::string& name)
	{
		m_pMesh = g_meshes->getMesh(name);
	}
	void Model::setMesh(demorender::Mesh *pMesh)
	{
		m_pMesh = pMesh;
	}

	void Model::bind(Shader* shader)
	{
		if (m_pMesh)
		{
			m_pMesh->bind(shader);
		}
		else
		{
			g_debug << "trying to bind a model but mesh is not set!" << std::endl;
		}
	}
	void Model::setTransform(const glm::mat4& transform)
	{
		m_transform = transform;
	}

	glm::mat4 Model::getTransform()
	{
		return m_transform;
	}

	Mesh* Model::getMesh()
	{
		return m_pMesh;
	}
}