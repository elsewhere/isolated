#pragma once

#include "../globals.h"

class Huuru : public democore::Scene
{

public:
	Huuru() {};
	~Huuru() {};
	
	void init();
	void update();
	void draw();

	void drawBackground();
	void drawFire();
	
private:
	demorender::Mesh* generateTreeMesh(int index);
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demorender::Light *m_light;
	demorender::Model *m_pModel;

	std::vector<demorender::Model*> m_trees;

};