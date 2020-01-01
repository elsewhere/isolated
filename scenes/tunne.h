#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Tunne : public democore::Scene
{

public:
	Tunne():m_pMesh(nullptr) {};
	~Tunne() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class TunneParticles : public demofx::GPUParticleSystem
	{
	public:
		TunneParticles();
		~TunneParticles();

		void setInitialData() override;



	};

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
//	demorender::Model *m_pSkybox;

	std::unique_ptr<TunneParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};