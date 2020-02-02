#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../render/ShadowMap.h"

class ShadowTest2 : public democore::Scene
{
public:
	ShadowTest2() {};
	~ShadowTest2() {};
	
	void init();
	void update();
	void draw();

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

	void drawGeometry(bool shadowPass);
	void drawTerrain();

	void updateLights();

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Mesh* m_terrain = nullptr;

	demorender::Camera *m_camera;

	demorender::Mesh* m_thingMesh = nullptr;
	std::vector<Thing*> m_things;

	demorender::Light m_pointLight;

	std::unique_ptr<demorender::ShadowMap> m_shadowMap;
	std::unique_ptr<demorender::LineRenderer> m_lines;


};