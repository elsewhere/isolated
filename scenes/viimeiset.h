#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Viimeiset : public democore::Scene
{

public:
	Viimeiset():m_pMesh(nullptr) {};
	~Viimeiset() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	void drawGround();

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
//	demorender::Model *m_pSkybox;

};