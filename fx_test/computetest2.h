#pragma once

#include "../globals.h"

class ComputeTest2 : public democore::Scene
{
public:
	ComputeTest2() {};
	~ComputeTest2() {};
	
	void init();
	void update();
	void draw();
	
private:
#pragma pack(push, 1)
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec4 color;
		float energy;
		float maxEnergy;
	};
#pragma pack(pop)

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;

	demorender::Buffer* m_buffer1;
	demorender::Buffer* m_buffer2;
	int m_particleCount;
};