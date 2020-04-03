#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/GPUParticleSystem.h"
#include "../fx/Feedback.h"

class Korona2 : public democore::Scene
{

public:
	Korona2() {};
	~Korona2() {};
	
	void init();
	void update();
	void draw(RenderPass pass);

	void debug() override;
	
private:

	class Korona2Particles : public demofx::GPUParticleSystem
	{
	public:
		Korona2Particles();
		~Korona2Particles();

		void setInitialData() override;
	};

	void drawFeedback();

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

	std::unique_ptr<demofx::Feedback> m_feedback;
	std::unique_ptr<Korona2Particles> m_particles;
	std::unique_ptr<demorender::LineRenderer> m_lines;

};