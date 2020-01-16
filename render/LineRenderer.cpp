#include "LineRenderer.h"
#include "../glm/gtx/transform.hpp"


namespace demorender
{

	LineRenderer::LineRenderer()
	{
		m_mesh = std::make_unique<Mesh>();
		m_dirty = true;
	}

	LineRenderer::~LineRenderer()
	{

	}

	void LineRenderer::clear()
	{
		m_points.clear();
		m_dirty = true;
	}

	void LineRenderer::startNewBatch()
	{
		//copy empy vector 
		std::vector<std::pair<glm::vec3, glm::vec4>> v;
		m_points.push_back(v); 
	}

	void LineRenderer::addPoint(const glm::vec3& point, glm::vec4 color)
	{
		if (m_points.size() > 0)
		{
			auto& v = m_points.at(m_points.size() - 1); //pushing into this vector
			v.push_back(std::make_pair(point, color));
			m_dirty = true;
		}
		else
		{
			g_error.log("trying to add a point to line renderer with no batches");
		}
	}

	void LineRenderer::updateMesh()
	{
		//this is bad and slow but it's only meant for debugging use anyway
		std::vector<Mesh::Vertex> temp;

		for (auto& vector : m_points)
		{
			for (auto& v : vector)
			{
				Mesh::Vertex vertex;
				vertex.position = v.first;
				vertex.color = v.second;

				vertex.uv = glm::vec2(0.f);
				vertex.normal = glm::vec3(0.f);

				temp.push_back(vertex);
			}
		}

		m_mesh->generate(Mesh::Usage::STATIC, &temp, nullptr, false);
		m_mesh->setStreamFlags(Mesh::COLOR_STREAM | Mesh::VERTEX_STREAM);

	}

	void LineRenderer::draw(Camera* pCamera, Mode mode)
	{
		if (m_dirty)
		{
			updateMesh();
			m_dirty = false;
		}

		Shader& s = g_shaders->getShader("debug_lines");
		s.bind();

		glm::mat4 model(1.f);
		s.setUniformMatrix4fv("camera", 1, false, (float *)&pCamera->getCameraMatrix());
		s.setUniformMatrix4fv("model", 1, false, (float *)&model);

		m_mesh->bind(&s);

		m_mesh->draw(mode == Mode::LINES ? Mesh::RenderMode::LINES : Mesh::RenderMode::LINE_STRIP);
	}
}