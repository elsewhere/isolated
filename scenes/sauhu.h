#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Sauhu : public democore::Scene
{

public:
	Sauhu():m_pMesh(nullptr) {};
	~Sauhu() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	class SauhuParticles : public demofx::GPUParticleSystem
	{
	public:
		SauhuParticles();
		~SauhuParticles();

		void setInitialData() override;



	};

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
//	demorender::Model *m_pSkybox;

	std::unique_ptr<SauhuParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};