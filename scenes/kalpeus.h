#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"

class Kalpeus : public democore::Scene
{

public:
	Kalpeus():m_pMesh(nullptr) {};
	~Kalpeus() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class Flower
	{
	public:
		Flower();
		~Flower();

		void draw(demorender::Camera* camera);
		void update();

	private:
		class Petal
		{
		public:
			Petal(const glm::vec3& startPoint, const glm::vec3& endPoint, float petalAngle);
			~Petal();

			std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec4>> m_debugLines;

		};

		void createMesh();
		std::vector<glm::vec3> m_points;
		glm::mat4 m_transform;

		glm::vec3 m_color;

		demorender::Mesh* m_pentaMesh;
		std::vector<Petal*> m_petals;

		std::unique_ptr<demorender::LineRenderer> m_lines;

	};

	std::vector<Flower*> m_flowers;

	void createMesh();
	void drawTerrain();
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;

	float m_lightValue = 0.f;
	glm::vec3 m_lightPos;
	float m_lightRadius = 100.f;

	std::unique_ptr<demorender::LineRenderer> m_lines;


};