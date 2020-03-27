#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Maailma : public democore::Scene
{

public:
	Maailma():m_pMesh(nullptr) {};
	~Maailma() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	class MaailmaParticles : public demofx::GPUParticleSystem
	{
	public:
		MaailmaParticles();
		~MaailmaParticles();

		void setInitialData() override;
	};

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;


	void drawBackground();
	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
	int m_numParticles;
	demorender::Model *m_pSkybox;

	std::unique_ptr<MaailmaParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};