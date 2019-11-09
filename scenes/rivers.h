#pragma once

#include "../globals.h"

class Rivers  : public democore::Scene
{

public:
	Rivers() {};
	~Rivers() {};
	
	void init();
	void update();
	void draw();
	
private:
/*
	class CPUParticleSystemDOF : public demorender::CPUParticleSystem
	{
	public:
		CPUParticleSystemDOF();
		~CPUParticleSystemDOF();

		void initParticle(int index);
		void setInitialValues() override;
		void update() override;
		void draw(float focusDistance, demorender::Camera *pCamera);
	};
*/
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;
//	CPUParticleSystemDOF *m_pParticles;

};