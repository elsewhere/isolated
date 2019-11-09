#pragma once

#include "../globals.h"

namespace demofx
{
	class Cloth;
}

class Midaftann : public democore::Scene
{

public:
	Midaftann():m_pMesh(nullptr) {};
	~Midaftann() {};
	
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

	class demofx::Cloth *m_pFlag;

};