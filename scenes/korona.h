#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

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

	void drawLines();
	void drawMoon();

	glm::vec3 m_moonCameraPosition;
	glm::vec3 m_moonCameraTarget;
	glm::vec3 m_moonCameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMoon;
	demorender::Mesh *m_pLines;
		//	demorender::Model *m_pSkybox;

	std::unique_ptr<KoronaParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};