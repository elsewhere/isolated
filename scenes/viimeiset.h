#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"
#include "../fx/CPUParticleSystem.h"

#include "../core/Analyzer.h"

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
	class WindParticles : public demofx::GPUParticleSystem
	{
	public:
		WindParticles();
		~WindParticles();

		void setInitialData() override;
	};

	class Sun : public demofx::CPUParticleSystem
	{
	public:
		Sun();
		~Sun();

		void setInitialValues() override;
		void update() override;
	};

	void drawBackground();
	void drawGround();

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
	demorender::Model *m_pSkybox;

	std::unique_ptr<WindParticles> m_particles;
	std::unique_ptr<demofx::CPUParticleSystem> m_sun;
	std::unique_ptr<democore::Analyzer> m_analyzer;

	float m_sum = 0.f;

};