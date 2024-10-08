#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"
#include "../fx/CPUParticleSystem.h"
#include "../core/Analyzer.h"

class Korona : public democore::Scene
{

public:
	Korona() {};
	~Korona() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	class KoronaParticles : public demofx::GPUParticleSystem
	{
	public:
		KoronaParticles();
		~KoronaParticles();

		void setInitialData() override;
	};

	class Sun : public demofx::CPUParticleSystem
	{
	public:
		Sun();
		~Sun();

		void setInitialValues() override;
		void update() override;
		void setPos(float t) { m_pos = t;  }

		float m_pos;
	};


	void drawLines();
	void drawMoon();
	void drawGround();

	glm::vec3 m_moonCameraPosition;
	glm::vec3 m_moonCameraTarget;
	glm::vec3 m_moonCameraUp;

	glm::vec3 m_groundCameraPosition;
	glm::vec3 m_groundCameraTarget;
	glm::vec3 m_groundCameraUp;

	demorender::Camera *m_moonCamera;
	demorender::Camera *m_groundCamera;

	demorender::Mesh *m_pGround;
	demorender::Mesh *m_pMoon;
	demorender::Mesh *m_pLines;
		//	demorender::Model *m_pSkybox;

	std::unique_ptr<KoronaParticles> m_particles;
	std::unique_ptr<Sun> m_sun;
	std::unique_ptr<democore::Analyzer> m_analyzer;


};