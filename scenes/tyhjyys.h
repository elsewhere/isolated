#pragma once

#include "../globals.h"
#include "../render/LineRenderer.h"

class Tyhjyys : public democore::Scene
{

public:
	Tyhjyys():m_pMesh(nullptr) {};
	~Tyhjyys() {};
	
	void init();
	void update();
	void draw();

	void debug() override;
	
private:

	class TyhjyysLight
	{
		public:
			TyhjyysLight();
			~TyhjyysLight();

			void update();
			void draw();

			void debug();

			void setPos(const glm::vec3& pos) { m_position = pos; };
			void setIntensity(float intensity) { m_intensity = intensity; }
	
		private:
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
	std::unique_ptr<TyhjyysLight> m_light;

	std::unique_ptr<demorender::LineRenderer> m_lines;

};