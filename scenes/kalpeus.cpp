#include "Kalpeus.h"
#include "../render/MeshBuilder.h"
#include "../glm/gtx/transform.hpp"

#include "../render/LineRenderer.h"

using namespace democore;
using namespace demomath;
using namespace demorender;
using namespace glm;

namespace
{
	static const std::string skyboxTexture = "nebula";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ground
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flower
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Kalpeus::Flower::Petal::Petal(Flower* parent, const glm::vec3& startPoint, const glm::vec3& endPoint, const float petalAngle):
	m_startPoint(startPoint),
	m_endPoint(endPoint),
	m_parent(parent)
{
	g_params->useNamespace("kalpeus");
	demomath::Range openingAngle = g_params->get<demomath::Range>("petalangle");

	demomath::Range startPowerRange = g_params->get<demomath::Range>("petalstartpower");
	demomath::Range endPowerRange = g_params->get<demomath::Range>("petalendpower");
	demomath::Range endRotation = g_params->get<demomath::Range>("petalendrotation");

	const float angle = openingAngle.getRandomValue();
	const float startPower = startPowerRange.getRandomValue();
	const float endPower = endPowerRange.getRandomValue();

	const float angle1 = petalAngle - angle;
	const float angle2 = petalAngle + angle;
	m_startDirection1 = glm::vec3(sinf(angle1), cosf(angle1), 0.f) * startPower;
	m_startDirection2 = glm::vec3(sinf(angle2), cosf(angle2), 0.f) * startPower;
	m_endDirection = glm::vec3(sinf(petalAngle), cosf(petalAngle), 0.f) * endPower;

	glm::mat4 endTransform = glm::rotate(endRotation.getRandomValue(), Math::randVectSphere());
	m_endDirection = Math::transform(m_endDirection, endTransform);

	m_debugLines.push_back(std::make_tuple(m_startPoint, startPoint + m_startDirection1, glm::vec4(1.f, 0.f, 0.f, 1.f)));
	m_debugLines.push_back(std::make_tuple(m_startPoint, startPoint + m_startDirection2, glm::vec4(1.f, 0.f, 0.f, 1.f)));
	m_debugLines.push_back(std::make_tuple(m_endPoint, endPoint + m_endDirection, glm::vec4(1.f, 0.f, 0.f, 1.f)));

	createMesh();
}

void Kalpeus::Flower::Petal::createMesh()
{
	MeshBuilder builder;

	if (m_parent->getType() == Type::STATIC)
	{
		const int resolution = 10;
		const float step = 1.f / resolution;

		std::vector<glm::vec3> leftEdge;
		std::vector<glm::vec3> rightEdge;

		for (float t = 0.f; t < 1.f; t += step)
		{
			glm::vec3 leftPoint = Math::evaluateBezier<glm::vec3>(
				m_startPoint,
				m_startPoint + m_startDirection1,
				m_endPoint - m_endDirection,
				m_endPoint, t);

			glm::vec3 rightPoint = Math::evaluateBezier<glm::vec3>(
				m_startPoint,
				m_startPoint + m_startDirection2,
				m_endPoint - m_endDirection,
				m_endPoint, t);

			leftEdge.push_back(leftPoint);
			rightEdge.push_back(rightPoint);
		}
		for (size_t i = 0; i < leftEdge.size() - 1; i++)
		{
			m_debugLines.push_back(std::make_tuple(leftEdge[i], leftEdge[i + 1], glm::vec4(1.f, 1.f, 1.f, 1.f)));
			m_debugLines.push_back(std::make_tuple(rightEdge[i], rightEdge[i + 1], glm::vec4(1.f, 1.f, 1.f, 1.f)));
		}

		builder.start(false);
		for (size_t i = 0; i < leftEdge.size(); i++)
		{
			builder.addVertex(leftEdge[i]);
			builder.addVertex(rightEdge[i]);
		}
		builder.end();
		m_mesh = builder.getMesh(Mesh::Usage::STATIC);
	}
	else if (m_parent->getType() == Type::DYNAMIC)
	{
		builder.start(false);
		builder.addVertex(m_startPoint);
		builder.addVertex(m_startPoint + m_startDirection1);
		builder.addVertex(m_startPoint);
		builder.addVertex(m_startPoint + m_startDirection2);
		builder.addVertex(m_endPoint - m_endDirection);
		builder.addVertex(m_endPoint);
		builder.end();
		m_mesh = builder.getMesh(Mesh::Usage::STREAM);
	}
}

void Kalpeus::Flower::Petal::draw(glm::mat4 cameraMatrix, const glm::mat4& transform, glm::mat4 lightMatrix)
{
	if (m_parent->getType() == Type::STATIC)
	{
		Shader& s = g_shaders->getShader("flower");

		s.bind();
		glm::vec4 color = m_parent->getColor();
		color.a = 1.f;

		s.setUniform1i("shadowMap", 1);
		s.setUniform4fv("color", 1, (float *)&color);
		s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&cameraMatrix); GL_DEBUG;
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&transform);
		s.setUniformMatrix4fv("lightMatrix", 1, GL_FALSE, (float *)&lightMatrix);

		m_mesh->bind(&s);
		m_mesh->draw(demorender::Mesh::RenderMode::TRIANGLE_STRIP);
	}
	else
	{
/*
		Shader& s = g_shaders->getShader("dynamicflower");

		s.bind(); 
		glm::vec4 color = m_parent->getColor();
		color.a = 1.f;
		s.setUniform4fv("flowerColor", 1, (float *)&color);
		s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&transform);
		s.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, (float *)&pCamera->getViewMatrix()); GL_DEBUG;
		s.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, (float *)&pCamera->getProjectionMatrix()); GL_DEBUG;
		m_mesh->bind(&s);
		m_mesh->draw(demorender::Mesh::RenderMode::TRIANGLES_ADJACENCY);
*/
	}
}

void Kalpeus::Flower::Petal::update()
{
	if (m_parent->getType() == Flower::Type::DYNAMIC)
	{

		MeshBuilder builder;
		builder.start(false);
		builder.addVertex(m_startPoint);
		builder.addVertex(m_startPoint + m_startDirection1);
		builder.addVertex(m_startPoint);
		builder.addVertex(m_startPoint + m_startDirection2);
		builder.addVertex(m_endPoint - m_endDirection);
		builder.addVertex(m_endPoint);
		builder.end();

//		auto vertices = builder.getVertices();
//		m_mesh->updateVertices((void *)&vertices, 6 * 3 * sizeof(float));

		//		m_mesh = builder.getMesh(Mesh::Usage::STATIC);// m_mesh->updateVertices((void *)&vertices, 6 * 3 * sizeof(float));

	}
}

Kalpeus::Flower::Flower(bool dynamic):
	m_pentaMesh(nullptr),
	m_type(dynamic ? Type::DYNAMIC : Type::STATIC)
{
	int count = 5; 

	float a = Math::randFloat() * 2 * 3.141592f;
	for (int i = 0; i < count; i++)
	{
		glm::vec3 p = glm::vec3(sinf(a), cosf(a), 0);

		m_points.push_back(p);

		Petal* petal = new Petal(this, glm::vec3(0.f), p, a);
		m_petals.push_back(petal);

		a += (2 * 3.141592f) / count;
	}

	const float angle = Math::randFloat(0, 2 * 3.141592f);
	const float radius = g_params->get<demomath::Range>("flowerradius").getRandomValue();
	const float height = g_params->get<demomath::Range>("flowerheight").getRandomValue();
	const float scale = g_params->get<demomath::Range>("flowerscale").getRandomValue();
	glm::vec3 flowerPos = glm::vec3(sinf(angle) * radius, height, cosf(angle) * radius);

	glm::vec3 hue = (m_type == Type::STATIC) ? glm::vec3(1.f) : glm::vec3(1.f, 0.f, 0.f);// glm::normalize(glm::vec3(Math::randFloat(), Math::randFloat(), Math::randFloat()));

	float alpha = g_params->get<demomath::Range>("flowerpentaalpha").getRandomValue();

	m_color = glm::vec4(hue, alpha);
	glm::vec3 rotAngle = Math::randVectSphere();

	rotAngle.y = 1.f;
	rotAngle = glm::normalize(rotAngle);
	
	m_transform = glm::translate(flowerPos) * glm::scale(glm::vec3(scale)) * glm::rotate(Math::randFloat() * 90.f, rotAngle);

	m_lines = std::make_unique<demorender::LineRenderer>();
	m_lines->startNewBatch();
/*
	for (const auto& p : m_points)
	{
		m_lines->addPoint(flowerPos, glm::vec4(1.f));
		glm::vec3 worldPos = Math::transform(p, m_transform);
		m_lines->addPoint(worldPos, glm::vec4(1.f));
	}
*/
	for (const auto& p : m_petals)
	{
		for (const auto& l : p->m_debugLines)
		{
			glm::vec3 startPoint = Math::transform(std::get<0>(l), m_transform);
			glm::vec3 endPoint = Math::transform(std::get<1>(l), m_transform);
			glm::vec4 color = std::get<2>(l);

			m_lines->addPoint(startPoint, color);
			m_lines->addPoint(endPoint, color);
			
		}
	}
	createMesh();


}

Kalpeus::Flower::~Flower()
{

}

void Kalpeus::Flower::createMesh()
{
	const float phi = 1.618f;

	//end points
	glm::vec3 v0 = m_points[0];
	glm::vec3 v1 = m_points[1];
	glm::vec3 v2 = m_points[2];
	glm::vec3 v3 = m_points[3];
	glm::vec3 v4 = m_points[4];

	const float ratio = 1.f - 1.f / phi;

	//middle points
	glm::vec3 a = v0 + (v3 - v0) * ratio;
	glm::vec3 b = v0 + (v2 - v0) * ratio;
	glm::vec3 c = v1 + (v3 - v1) * ratio;
	glm::vec3 d = v2 + (v4 - v2) * ratio;
	glm::vec3 e = v3 + (v0 - v3) * ratio;

	//pentagram polygons
/*
	MeshBuilder builder;
	builder.start(false);

	//star points
	builder.addTriangleVertex(v0, b, a);
	builder.addTriangleVertex(v1, c, b);
	builder.addTriangleVertex(v2, d, c);
	builder.addTriangleVertex(v3, e, d);
	builder.addTriangleVertex(v4, a, e);

	//center
	builder.addTriangleVertex(a, d, e);
	builder.addTriangleVertex(a, b, d);
	builder.addTriangleVertex(b, c, d);

	builder.end();
	*/
	MeshBuilder builder;
	builder.generateCube(0.3f);
	m_pentaMesh = builder.getMesh(Mesh::Usage::STATIC);


}

void Kalpeus::Flower::draw(glm::mat4 cameraMatrix, glm::mat4 lightMatrix)
{
	Shader& s = g_shaders->getShader("flower");

	s.bind();
	s.setUniform4fv("color", 1, (float *)&m_color);
	s.setUniformMatrix4fv("cameraMatrix", 1, GL_FALSE, (float *)&cameraMatrix); GL_DEBUG;
	s.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, (float *)&m_transform);
	s.setUniformMatrix4fv("lightMatrix", 1, GL_FALSE, (float *)&lightMatrix);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pentaMesh->bind(&s);
	m_pentaMesh->draw();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	for (const auto f : m_petals)
	{
//		f->draw(cameraMatrix, m_transform, lightMatrix);
	}


//	m_lines->draw(pCamera, LineRenderer::Mode::LINES);

}

void Kalpeus::Flower::update()
{
	if (m_type == Type::DYNAMIC)
	{
		for (auto& p : m_petals)
		{
			p->m_endDirection = Math::randVectSphere() * Math::randBetween(1.5, 6.f);
			p->update();
			
		}

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kalpeus
////////////////////////////////////////////////////////////////1////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Kalpeus::init()
{
	m_flowers.clear();
	g_params->useNamespace("kalpeus");
	m_camera = new demorender::Camera(1.f, 1000.f, 45.f);

	m_lines = std::make_unique<demorender::LineRenderer>();

	m_lines->startNewBatch();

	for (int i = 0; i < 300; i++)
	{
		//		float t = (i / 300.f - 0.5f) * 2.f;
		//		const float a = t * 15.f;
		//		glm::vec3 v = glm::vec3(sinf(a) * 10.f, t * 100.f, cosf(a) * 10.f);
		glm::vec3 v = Math::randVectSphere() * Math::randFloat(300.f);

		glm::vec4 c = i < 150 ? glm::vec4(1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_lines->addPoint(v, c);
	}
	
	
	int flowerCount = g_params->get<int>("flowercount");
	for (int i = 0; i < flowerCount; i++)
	{
		Flower *f = new Flower(false);// i < flowerCount / 2);
		m_flowers.push_back(f);
	}

	if (m_pMesh)
		delete m_pMesh;

	const int zres = 100;
	const int xres = 100;
	const float scale = 100.f;

	MeshBuilder builder;
	builder.generatePlane(xres, zres, scale);
	m_pMesh = builder.getMesh(Mesh::Usage::STATIC);



	m_shadowMap = std::make_unique<ShadowMap>();
}

void Kalpeus::update()
{
	g_params->useNamespace("kalpeus");

	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

	const float radius = 100.f;
	const float height = 30.f;

	const float a = m_pos * 2.f;
	
	glm::vec3 cameraStart = glm::vec3(0, 0, -240.f);
	glm::vec3 cameraEnd = glm::vec3(0, 0, 240.f);

	m_cameraPosition = cameraStart + (cameraEnd - cameraStart) * m_pos;
	m_cameraPosition.y = height;
	m_cameraPosition += glm::vec3(sinf(a), 0.f, cosf(a)) * radius;

	m_cameraTarget = m_cameraPosition * 0.000f;
	m_cameraTarget.y = 0.f;// ::vec3(0.f);
	
	for (auto f : m_flowers)
		f->update();
}

void Kalpeus::debug()
{
//	g_screenText.log<std::string>("lightPos", Math::toString(m_lightPos));
//	g_screenText.log<float>("m_lightValue", m_lightValue);
//	g_screenText.log<float>("m_lightRadius", m_lightRadius);
}

void Kalpeus::drawTerrain()
{
	glm::mat4 model = glm::mat4(1.f);

	Shader& s = g_shaders->getShader("plane");
	s.bind();

	g_textures->bindTexture("kivitesti", GL_TEXTURE0);
	g_textures->bindTexture(m_shadowMap->getDepthMapID(), GL_TEXTURE1);
	s.setUniform1i("texturemap", 0);
	s.setUniform1i("shadowMap", 1);

	s.setUniformMatrix4fv("camera", 1, false, (float *)&m_camera->getCameraMatrix());
	s.setUniformMatrix4fv("model", 1, false, (float *)&model);
	s.setUniformMatrix4fv("lightMatrix", 1, false, (float *)&m_shadowMap->getLightMatrix());

	m_pMesh->bind(&s);
	m_pMesh->draw();

}

void Kalpeus::draw()
{
	g_params->useNamespace("Kalpeus");

	m_shadowMap->bind();
	
	drawTerrain();
	for (auto f : m_flowers)
		f->draw(m_shadowMap->getLightMatrix(), glm::mat4(1.f));

	m_shadowMap->unbind();

	g_renderTargets->bindMain();
	/*
	m_camera->lookAt(m_cameraPosition,
		m_cameraTarget,
		m_cameraUp);

	drawTerrain();

	for (auto f : m_flowers)
		f->draw(m_camera->getCameraMatrix(), m_shadowMap->getLightMatrix());
		*/
//
	m_shadowMap->debugDraw();
//	const float focus = 0.1f;

	//	g_postProcess->addRadialGlow();
	//	g_postProcess->addLens(focus, m_camera);
	//	g_renderDebug->drawDepthTexture(g_renderTargets->getDepthTextureId("main"), m_camera, 512 + 256, 256, 512.f);

}
