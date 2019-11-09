#pragma once

#include "../globals.h"

class ComputeTest : public democore::Scene
{
public:
	ComputeTest() {};
	~ComputeTest() {};
	
	void init();
	void update();
	void draw();
	
private:

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	demorender::Camera *m_camera;

	GLuint m_texture;
};