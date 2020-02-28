#pragma once

#include "../globals.h"

#include "../render/CubemapRenderer.h"
#include "../render/LineRenderer.h"
#include "../render/ShadowMap.h"

class DynamicCubeTest : public democore::Scene
{
public:
	DynamicCubeTest() {};
	~DynamicCubeTest() {};
	
	void init();
	void update();
	void draw(Scene::RenderPass pass) override;

	void debug() override;
	
private:

	struct Thing
	{
		demorender::Mesh* pMesh;
		glm::mat4 transform;

		glm::vec4 color;

		glm::vec3 pos;
		float scale;
	};

	void drawGeometry(bool shadowPass, glm::mat4 cameraMatrix);
	void drawTerrain();
	void drawReflector();

	void updateLights();

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Mesh* m_terrain = nullptr;

	demorender::Camera *m_camera;

	demorender::Mesh* m_thingMesh = nullptr;
	demorender::Mesh* m_reflector = nullptr;
	std::vector<Thing*> m_things;

	glm::vec3 m_reflectorPosition;
	demorender::Light m_pointLight;


	std::unique_ptr<demorender::CubemapRenderer> m_cubemapRenderer;
	std::unique_ptr<demorender::LineRenderer> m_lines;


};