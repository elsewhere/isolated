#include "scene.h"

namespace democore
{

	Scene::Scene()
	{
		m_pos = 0.0f;
	}

	Scene::~Scene()
	{
		//empty virtual destructor
	}

	void Scene::setPosition(const float t)
	{
		m_pos = t;
	}

	void Scene::release()
	{
		//leave it empty, some effects might want to release something if they use a gigabyte of memory
		//or something and they're iterated, but probably not	
	}

	void Scene::debug()
	{

	}
}