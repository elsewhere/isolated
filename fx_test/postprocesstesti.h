#pragma once

#include "../globals.h"
#include "../render/Light.h"

class PostProcessTesti : public democore::Scene
{

public:
	PostProcessTesti() {};
	~PostProcessTesti() {};
	
	void init();
	void update();
	void draw();
	
private:
	void drawScene(const std::string& textureName);
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demorender::Light *m_light;
	demorender::Model *m_pModel;

};