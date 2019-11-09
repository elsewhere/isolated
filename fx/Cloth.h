#pragma once

#include "../Globals.h"

namespace demofx
{
	class Cloth
	{

	public:
		Cloth();
		~Cloth();

		void init(int xres, int yres, const glm::mat4& pointTransform);
		void update(const float dt);
		void draw(demorender::Camera *pCamera, const glm::mat4& modelMatrix);

		void setClothParams(float damping, float gravity);
		void setWindParams(float speed, float magnitude, float density);

	protected:
		class Point
		{
		public:
			Point();
			~Point();

			void update(const float dt);

			glm::vec3 m_totalForce;
			glm::vec3 m_position;
			glm::vec3 m_previousPosition;
			glm::vec3 m_acceleration;
			glm::vec4 m_color;
			glm::vec2 m_uv;
			glm::vec3 m_normal;

			bool m_stationary;
			static float sm_damping;
		};

		class Constraint
		{
		public:
			Constraint(Point* p1, Point* p2);
			~Constraint();

			void update();

			Point *m_point1;
			Point *m_point2;

			float m_restDistance;
		};

		void initRendering();
		void updateBuffer();

		glm::vec3 windFunc(const glm::vec3& pos);

		float m_timer;
		int m_xres, m_yres;
		Point **m_points;
		std::vector<Constraint*> m_constraints;

		unsigned int m_vao;
		unsigned int m_vbo;

		float m_windSpeed;
		float m_windMagnitude;
		float m_windDensity;

		float m_gravity;

#pragma pack(push, 1)
		struct ClothVertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::vec4 color;
		};
#pragma pack(pop)

	};

}