#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"

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

		void draw(demorender::Camera* camera);
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

			void draw(demorender::Camera* pCamera, const glm::mat4& transform);
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

	void createMesh();
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;


	std::unique_ptr<demorender::LineRenderer> m_lines;


};