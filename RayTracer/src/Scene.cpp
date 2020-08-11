#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_spheres = std::vector<Sphere>();
	}

	void Scene::AddSphere(Sphere sphere)
	{
		m_spheres.push_back(sphere);
	}

	void Scene::RenderScene()
	{
	}
}