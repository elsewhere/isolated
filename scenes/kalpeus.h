#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"

#include "../render/ShadowMap.h"

class Kalpeus : public democore::Scene
{

public:
	Kalpeus() {};
	~Kalpeus() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class Flower
	{
	public:
		Flower(bool dynamic);
		~Flower();

		void draw(glm::mat4 cameraMatrix, glm::mat4 lightMatrix);
		void update();

		glm::vec4 getColor() { return m_color; }

		enum class Type
		{
			STATIC,
			DYNAMIC,
		};

		Type getType() { return m_type; }

	private:
		class Petal
		{
		public:
			Petal(Flower* parent, const glm::vec3& startPoint, const glm::vec3& endPoint, float petalAngle);
			~Petal() {};

			void draw(glm::mat4 cameraMatrix, const glm::mat4& transform, glm::mat4 lightMatrix);
			void update();
			std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec4>> m_debugLines;

			void createMesh();
			demorender::Mesh* m_mesh;

			Flower* m_parent;
			glm::vec3 m_startPoint;
			glm::vec3 m_endPoint;
			glm::vec3 m_startDirection1;
			glm::vec3 m_startDirection2;
			glm::vec3 m_endDirection;
		};

		void createMesh();

		Type m_type;

		std::vector<glm::vec3> m_points;
		glm::mat4 m_transform;

		glm::vec4 m_color;

		demorender::Mesh* m_pentaMesh;
		std::vector<Petal*> m_petals;
		std::unique_ptr<demorender::LineRenderer> m_lines;
	};

	std::vector<Flower*> m_flowers;

	void drawTerrain();

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh* m_pMesh = nullptr;

	std::unique_ptr<demorender::ShadowMap> m_shadowMap;


	std::unique_ptr<demorender::LineRenderer> m_lines;


};