#pragma once

#include "../globals.h"

class Fjara : public democore::Scene
{

public:
	Fjara():m_pMesh(nullptr) {};
	~Fjara() {};
	
	void init();
	void update();
	void draw();
	
private:
	void createMesh();
	void drawBackground();
	void drawTerrain();
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
	demorender::Model *m_pSkybox;

};