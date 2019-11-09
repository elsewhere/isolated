#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"
#include "../fx/LensFlare.h"

class Ikuisuus : public democore::Scene
{

public:
	Ikuisuus():m_pMesh(nullptr) {};
	~Ikuisuus() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class IkuisuusLight
	{
		public:
			IkuisuusLight();
			~IkuisuusLight();

			void update();
			void draw();

			void debug();

			void setPos(const glm::vec3& pos) { m_position = pos; };
			void setIntensity(float intensity) { m_intensity = intensity; }
	
		private:
			std::unique_ptr<demofx::LensFlare> m_flare;
			glm::vec3 m_position = glm::vec3(0.f);
			float m_intensity;

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

	float m_lightValue = 0.f;
	glm::vec3 m_lightPos;
	float m_lightRadius = 100.f;
	std::unique_ptr<IkuisuusLight> m_light;

	std::unique_ptr<demorender::LineRenderer> m_lines;

};