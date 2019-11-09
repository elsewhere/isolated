#include "Cloth.h"
#include "../Globals.h"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtx/norm.hpp"

using namespace demorender;

namespace demofx
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Point
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float Cloth::Point::sm_damping = 0.1f;

	Cloth::Point::Point():
		m_position(glm::vec3(0.f)),
		m_previousPosition(glm::vec3(0.f)),
		m_acceleration(glm::vec3(0.f)),
		m_totalForce(glm::vec3(0.f)),
		m_color(glm::vec4(1.f)),
		m_normal(glm::vec3(0.f)),
		m_uv(glm::vec2(0.f)),
		m_stationary(false)
	{
//		m_color = glm::vec4(glm::vec3(Math::randFloat(0.6f, 1.f)), 1.f);
		m_color = glm::vec4(1.f);

	}

	Cloth::Point::~Point()
	{

	}

	void Cloth::Point::update(const float dt)
	{
		if (m_stationary)
		{
			m_acceleration = glm::vec3(0.f);
			m_previousPosition = m_position;
		}
		else
		{
			m_acceleration = m_totalForce;

			glm::vec3 temp = m_position;
			m_position += (m_position - m_previousPosition) * (1.f - sm_damping) + m_acceleration * dt;
			m_previousPosition = m_position;
		}

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constraint
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Cloth::Constraint::Constraint(Point* p1, Point *p2):
		m_point1(p1),
		m_point2(p2)
	{
		m_restDistance = glm::length(p1->m_position - p2->m_position);
		
	}

	Cloth::Constraint::~Constraint()
	{
		
	}

	void Cloth::Constraint::update()
	{
		glm::vec3 v = m_point2->m_position - m_point1->m_position;
		float dist = glm::length(v);

		glm::vec3 offset = v * (1.f - m_restDistance / dist);

		if (!m_point1->m_stationary)
			m_point1->m_position += offset * 0.5f;
		if (!m_point2->m_stationary)
			m_point2->m_position -= offset * 0.5f;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cloth
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Cloth::Cloth():
		m_points(nullptr),
		m_xres(0),
		m_yres(0),
		m_timer(0.f),
		m_vbo(-1),
		m_vao(-1),
		m_windSpeed(11.666f),
		m_windMagnitude(4.f),
		m_windDensity(2.3f),
		m_gravity(-1.5)
	{

	}

	Cloth::~Cloth()
	{
		delete[] m_points;

	}

	void Cloth::initRendering()
	{
		glGenVertexArrays(1, &m_vao); GL_DEBUG;

		glBindVertexArray(m_vao); GL_DEBUG;
		glGenBuffers(1, &m_vbo); GL_DEBUG;
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); GL_DEBUG;

		Shader& s = g_shaders->getShader("effect_flag");

		int vertexCount = m_xres * m_yres * 6;

		GLuint vertexAttrib = s.attrib("vertexPosition");
		GLuint colorAttrib = s.attrib("vertexColor");
		GLuint normalAttrib = s.attrib("vertexNormal");
		GLuint uvAttrib = s.attrib("vertexUv");

		glEnableVertexAttribArray(vertexAttrib); GL_DEBUG;
		glEnableVertexAttribArray(normalAttrib); GL_DEBUG;
		glEnableVertexAttribArray(colorAttrib); GL_DEBUG;
		glEnableVertexAttribArray(uvAttrib); GL_DEBUG;
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, position))); GL_DEBUG;
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, normal))); GL_DEBUG;
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, color))); GL_DEBUG;
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, uv))); GL_DEBUG;
	}

	void Cloth::init(int xres, int yres, const glm::mat4& pointTransform)
	{
		m_xres = xres;
		m_yres = yres;

		initRendering();

		m_points = new Point*[m_yres];

		for (int y = 0; y < m_yres; y++)
		{
			float yt = (float)y / (float)m_yres;
			m_points[y] = new Point[m_xres];

			for (int x = 0; x < xres; x++)
			{
				float xt = (float)x / (float)m_xres;

				Point& p = m_points[y][x];

				//xy-taso [-1, 1]

				glm::vec3 pos = glm::vec3(xt * 2.f - 1.f, yt * 2.f - 1.f, 0.f);


				glm::vec4 temp = glm::vec4(pos, 1.0) * pointTransform;
				p.m_position = glm::vec3(temp.x, temp.y, temp.z);
				p.m_previousPosition = pos;
				p.m_acceleration = glm::vec3(0.f);
				p.m_stationary = x < 2;
				p.m_uv = glm::vec2(xt, yt);
			}
		}
		//init constraints

		for (int y = 0; y < m_yres - 1; y++)
		{
			for (int x = 0; x < m_xres - 1; x++)
			{
				//alaspäin
				if (y < m_yres - 1)
				{
					Constraint* c = new Constraint(&m_points[y][x], &m_points[y + 1][x]);
					m_constraints.push_back(c);
				}
				//oikealle
				if (x < m_xres - 1)
				{
					Constraint* c = new Constraint(&m_points[y][x], &m_points[y][x + 1]);
					m_constraints.push_back(c);
				}
				//oikealle pitkälle
				if (x < m_xres - 2)
				{
					Constraint* c = new Constraint(&m_points[y][x], &m_points[y][x + 2]);
					m_constraints.push_back(c);
				}
				//alas pitkälle
				if (y < m_yres - 2)
				{
					Constraint* c = new Constraint(&m_points[y][x], &m_points[y + 2][x]);
					m_constraints.push_back(c);
				}
				//vinoon vasemmalle
				if (x < m_xres - 1 && y < m_yres - 1)
				{
					Constraint* c1 = new Constraint(&m_points[y][x], &m_points[y + 1][x + 1]);
					m_constraints.push_back(c1);
					Constraint* c2 = new Constraint(&m_points[y][x + 1], &m_points[y + 1][x]);
					m_constraints.push_back(c1);

				}
				if (x < m_xres - 2 && y < m_yres - 2)
				{
					Constraint *c1 = new Constraint(&m_points[y][x], &m_points[y + 2][x + 2]);
					m_constraints.push_back(c1);
					Constraint *c2 = new Constraint(&m_points[y][x + 2], &m_points[y + 2][x]);
					m_constraints.push_back(c2);
				}
			}
		}
	}

	void Cloth::setClothParams(float damping, float gravity)
	{
		Point::sm_damping = damping;
		m_gravity = gravity;
	}
	void Cloth::setWindParams(float speed, float magnitude, float density)
	{
		m_windSpeed = speed;
		m_windMagnitude = magnitude;
		m_windDensity = density;
	}
	glm::vec3 Cloth::windFunc(const glm::vec3& pos)
	{
		glm::vec3 w = glm::vec3(sinf(pos.x * 15.f * m_windDensity + m_timer * 0.6f * m_windSpeed) + 0.4f,
								cosf(pos.y * 11.f * m_windDensity + m_timer * 0.7f * m_windSpeed),
								sinf(pos.z * 12.f * m_windDensity + m_timer * 1.3f * m_windSpeed) * 1.5f + cosf(pos.z * 7.f * m_windDensity + m_timer * 0.5f * m_windSpeed));

//		return glm::normalize(w) * magnitude;
		return w * m_windMagnitude;
	}

	void Cloth::update(const float dt)
	{
		const int iterations = 15;

		m_timer += dt;

		//tuuli ja painovoima + reset
		glm::vec3 gravity = glm::vec3(0.f, m_gravity, 0.f);
		glm::vec3 zero = glm::vec3(0.f);

		for (int y = 0; y < m_yres; y++)
		{
			for (int x = 0; x < m_xres; x++)
			{
				Point& p = m_points[y][x];
				p.m_totalForce = gravity + windFunc(p.m_position);
				p.m_normal = zero;
			}
		}

		//animate
		for (int y = 0; y < m_yres; y++)
		{
			for (int x = 0; x < m_xres; x++)
			{
				m_points[y][x].update(dt);
			}
		}
		//constraintit
		for (int i = 0; i < iterations; i++)
		{
			for (auto& c : m_constraints)
			{
				c->update();
			}
		}

		//normaalit
		for (int y = 0; y < m_yres - 1; y++)
		{
			for (int x = 0; x < m_xres - 1; x++)
			{
				Point& v1 = m_points[y][x];
				Point& v2 = m_points[y][x + 1];
				Point& v3 = m_points[y + 1][x + 1];
				Point& v4 = m_points[y + 1][x];

				glm::vec3 n1 = glm::cross(v1.m_position - v3.m_position, v2.m_position - v3.m_position);
				glm::vec3 n2 = glm::cross(v2.m_position - v4.m_position, v3.m_position - v4.m_position);

				v1.m_normal += n1;
				v2.m_normal += n1;
				v3.m_normal += n1;

				v2.m_normal += n2;
				v3.m_normal += n2;
				v4.m_normal += n2;
			}
		}
		for (int y = 0; y < m_yres; y++)
		{
			for (int x = 0; x < m_xres; x++)
			{
				m_points[y][x].m_normal = glm::normalize(m_points[y][x].m_normal);
			}
		}
	}

	void Cloth::updateBuffer()
	{
		Shader& s = g_shaders->getShader("effect_flag");

		GLuint vertexAttrib = s.attrib("vertexPosition");
		GLuint colorAttrib = s.attrib("vertexColor");

		int vertexCount = m_xres * m_yres * 6;

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * vertexCount, 0, GL_DYNAMIC_DRAW);

		ClothVertex* writePtr = reinterpret_cast<ClothVertex*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ClothVertex) * vertexCount, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)); GL_DEBUG;

		if (writePtr == 0)
		{
			g_debug << "writeptr null in cloth system!" << std::endl;
			g_error.log("writeptr null in cloth");
		}
		else
		{
			for (int y = 0; y < m_yres - 1; y++)
			{
				for (int x = 0; x < m_xres - 1; x++)
				{
					Point& v1 = m_points[y][x];
					Point& v2 = m_points[y][x + 1];
					Point& v3 = m_points[y + 1][x + 1];
					Point& v4 = m_points[y + 1][x];

					writePtr->position = v1.m_position;
					writePtr->color = v1.m_color;
					writePtr->normal = v1.m_normal;
					writePtr->uv = v1.m_uv;
					writePtr++;
					writePtr->position = v2.m_position;
					writePtr->color = v2.m_color;
					writePtr->normal = v2.m_normal;
					writePtr->uv = v2.m_uv;
					writePtr++;
					writePtr->position = v3.m_position;
					writePtr->color = v3.m_color;
					writePtr->normal = v3.m_normal;
					writePtr->uv = v3.m_uv;
					writePtr++;
					writePtr->position = v1.m_position;
					writePtr->color = v1.m_color;
					writePtr->normal = v1.m_normal;
					writePtr->uv = v1.m_uv;
					writePtr++;
					writePtr->position = v3.m_position;
					writePtr->color = v3.m_color;
					writePtr->normal = v3.m_normal;
					writePtr->uv = v3.m_uv;
					writePtr++;
					writePtr->position = v4.m_position;
					writePtr->color = v4.m_color;
					writePtr->normal = v4.m_normal;
					writePtr->uv = v4.m_uv;
					writePtr++;
				}
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
	
	void Cloth::draw(demorender::Camera *pCamera, const glm::mat4& modelMatrix)
	{
		updateBuffer();
		//create buffers

		Shader& s = g_shaders->getShader("effect_flag");

		s.bind();
		s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&pCamera->getCameraMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&modelMatrix); GL_DEBUG;
		s.setUniform1i("surfaceTexture", 0);

		g_textures->bindTexture("fabric", GL_TEXTURE0);

		GLuint vertexAttrib = s.attrib("vertexPosition");
		GLuint normalAttrib = s.attrib("vertexNormal");
		GLuint colorAttrib = s.attrib("vertexColor");
		GLuint uvAttrib = s.attrib("vertexUv");

		glBindVertexArray(m_vao); GL_DEBUG;
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); GL_DEBUG;

		glEnableVertexAttribArray(vertexAttrib); GL_DEBUG;
		glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, position))); GL_DEBUG;

		glEnableVertexAttribArray(normalAttrib); GL_DEBUG;
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, normal))); GL_DEBUG;

		glEnableVertexAttribArray(colorAttrib); GL_DEBUG;
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, color))); GL_DEBUG;
		
		glEnableVertexAttribArray(uvAttrib); GL_DEBUG;
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), reinterpret_cast<void *>(offsetof(ClothVertex, uv))); GL_DEBUG;
		
		glDrawArrays(GL_TRIANGLES, 0, m_xres * m_yres * 6); GL_DEBUG;

		glDisableVertexAttribArray(vertexAttrib); GL_DEBUG;
		glDisableVertexAttribArray(colorAttrib); GL_DEBUG;
	}

}