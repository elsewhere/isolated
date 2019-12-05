#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Kasvot : public democore::Scene
{

public:
	Kasvot():m_pMesh(nullptr) {};
	~Kasvot() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class KasvotParticles : public demofx::GPUParticleSystem
	{
	public:
		KasvotParticles();
		~KasvotParticles();

		void setInitialData() override;



	};

	void createMesh();
	void drawBackground();
	void drawTerrain();
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
	demorender::Model *m_pSkybox;

	std::unique_ptr<KasvotParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};