#pragma once

#include "../globals.h"
#include "../fx/GPUParticleSystem.h"

class ParticleTesti3 : public democore::Scene
{
public:
	ParticleTesti3() {};
	~ParticleTesti3() {};
	
	void init();
	void update();
	void draw();
	
private:

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
	demofx::GPUParticleSystem *m_pParticles;

};