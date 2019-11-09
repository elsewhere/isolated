#pragma once

#include "../Globals.h"

namespace demorender
{
	class LineRenderer
	{
	public:

		enum class Mode
		{
			LINES,
			LINE_STRIP,
		};

		LineRenderer();
		~LineRenderer();

		void clear();

		void startNewBatch();
		void addPoint(const glm::vec3& point, glm::vec4 color = glm::vec4(1.f));
		void draw(demorender::Camera* pCamera, Mode mode = Mode::LINES);

	private:
		void updateMesh();
		std::unique_ptr<Mesh> m_mesh;
		std::vector<std::vector<std::pair<glm::vec3, glm::vec4>>> m_points;

		bool m_dirty = true;

	};

}