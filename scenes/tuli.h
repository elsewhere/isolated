#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"

class Tuli : public democore::Scene
{

public:
	Tuli():m_pMesh(nullptr) {};
	~Tuli() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	class TuliParticles : public demofx::GPUParticleSystem
	{
	public:
		TuliParticles();
		~TuliParticles();

		void setInitialData() override;



	};

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;

	demorender::Camera *m_camera;

	demorender::Mesh *m_pMesh;
//	demorender::Model *m_pSkybox;

	std::unique_ptr<TuliParticles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};