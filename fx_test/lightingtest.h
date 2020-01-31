#pragma once

#include "../globals.h"
#include "../render/Light.h"

class LightingTest : public democore::Scene
{

public:
	LightingTest() {};
	~LightingTest() {};
	
	void init();
	void update();
	void draw();
	
private:
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demorender::Light *m_light;
	demorender::Model *m_pModel;

};