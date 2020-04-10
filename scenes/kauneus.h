#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"
#include "../core/Analyzer.h"

class Kauneus : public democore::Scene
{

public:
	Kauneus():m_pMesh(nullptr) {};
	~Kauneus() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:



	class KauneusParticles : public demofx::GPUParticleSystem
	{
	public:
		KauneusParticles();
		~KauneusParticles();

		void setInitialData() override;



	};

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	void drawBackground();
	demorender::Mesh *m_pMesh;
	demorender::Model *m_pSkybox;

	std::unique_ptr<KauneusParticles> m_particles;
	std::unique_ptr<democore::Analyzer> m_analyzer;

	float m_sum = 0.f;
};