#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_spheres = std::vector<Sphere>();
		m_light = DirectionalLight({ 0,-1,0 }, { 1,1,1 }, 1.0f);
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
		glm::vec3 intensity = glm::vec3(0, 0, 0);

		HitInfo hitInformation;

		bool intersectsObject = TestIntersection(rayOrigin, ray, hitInformation);

		if (!intersectsObject) return intensity;

		// Cast shadow ray here
		glm::vec3 shadowRay = m_light.DirectionToLight(hitInformation.hitPosition);
		HitInfo occlusionInformation;
		bool isOccluded = TestIntersection(hitInformation.hitPosition, shadowRay, occlusionInformation);

		if (isOccluded) return intensity;

		intensity += m_light.Illumination(hitInformation.hitPosition, hitInformation.hitNormal, ray);

		return hitInformation.hitColor * intensity;
	}

	bool Scene::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation)
	{
		Sphere* hitSphere = nullptr;
		float closest = INFINITY;

		HitInfo tempInfo;

		for (int i = 0; i < m_spheres.size(); i++)
		{
			if (m_spheres[i].Intersects(rayOrigin, ray, tempInfo))
			{
				if (tempInfo.hitDistance < closest)
				{
					closest = tempInfo.hitDistance;
					hitInformation = tempInfo;
					hitSphere = &m_spheres[i];
				}
			}
		}

		if (hitSphere == nullptr) return false;

		return true;
	}
}