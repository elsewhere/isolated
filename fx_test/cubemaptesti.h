#pragma once

#include "../globals.h"
#include "../render/Light.h"

class CubemapTesti : public democore::Scene
{

public:
	CubemapTesti() {};
	~CubemapTesti() {};
	
	void init();
	void update();
	void draw();
	
private:
	void drawScene();
	void drawBackground();
	void drawMesh();

	demorender::Mesh *m_pMesh;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demorender::Light *m_light;
	demorender::Model *m_pModel;

};