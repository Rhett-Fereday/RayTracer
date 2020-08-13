#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_spheres = std::vector<Sphere>();
		m_light = DirectionalLight({ 0,1,0 }, { 1,1,1 }, 1.0f);
	}

	void Scene::AddSphere(Sphere sphere)
	{
		m_spheres.push_back(sphere);
	}

	void Scene::RenderScene()
	{
		m_camera.Render(this);
	}
	
	void Scene::SaveScene(const char* filename)
	{
		m_camera.GetRenderTarget().save(filename);
	}

	glm::vec3 Scene::TraceRay(const glm::vec3& rayOrigin, const glm::vec3& ray, glm::vec3 rayIntensity, const int& depth)
	{
		Sphere* hitSphere = nullptr;
		float closest = INFINITY;

		HitInfo tempInfo, finalInfo;

		for (int i = 0; i < m_spheres.size(); i++)
		{
			if (m_spheres[i].Intersects(rayOrigin, ray, tempInfo, m_camera.m_worldToCamera))
			{
				float dist = distance(rayOrigin, tempInfo.hitPosition);

				if (dist < closest)
				{
					closest = dist;
					finalInfo = tempInfo;
					hitSphere = &m_spheres[i];
				}
			}
		}

		glm::vec3 intensity = glm::vec3(0, 0, 0);

		if (hitSphere == nullptr) return intensity;

		//for (int i = 0; i < lights.size(); i++) intensity += lights[i]->illumination(phit, nhit, ray);

		//glm::vec3 color = object->color() * intensity;

		intensity += m_light.illumination(finalInfo.hitPosition, finalInfo.hitNormal, ray);

		return finalInfo.hitColor * intensity;
	}
}