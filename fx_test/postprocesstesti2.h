#pragma once

#include "../globals.h"

class PostProcessTesti2 : public democore::Scene
{

public:
	PostProcessTesti2() {};
	~PostProcessTesti2() {};
	
	void init();
	void update();
	void draw();
	
private:
	void drawScene(const std::string& textureName);
	void drawBackground();
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demorender::Light *m_light;
	demorender::Model *m_pModel;
	demorender::Model *m_pSkybox;

};