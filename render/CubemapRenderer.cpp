#include "CubemapRenderer.h"
#include "../glm/gtx/transform.hpp"


namespace demorender
{

	CubemapRenderer::CubemapRenderer()
	{
		m_basis.push_back(std::make_pair(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
		m_basis.push_back(std::make_pair(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
		m_basis.push_back(std::make_pair(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
		m_basis.push_back(std::make_pair(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)));
		m_basis.push_back(std::make_pair(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)));
		m_basis.push_back(std::make_pair(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f)));
	}

	CubemapRenderer::~CubemapRenderer()
	{
	}

	void CubemapRenderer::setPosition(const glm::vec3& position, float nearPlane, float farPlane)
	{
		const float aspect = 1.f;
		m_transforms.clear();

		glm::mat4 projection = glm::perspective(glm::radians(90.f), aspect, nearPlane, farPlane);

		for (const auto& d : m_basis)
			m_transforms.push_back(projection * glm::lookAt(position, position + d.first, d.second));
	}

	const glm::mat4& CubemapRenderer::getTransform(Cubemap::Side side)
	{
		return m_transforms[side];
	}

}