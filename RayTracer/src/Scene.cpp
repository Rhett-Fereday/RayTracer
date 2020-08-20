#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_spheres = std::vector<Sphere>();
		m_light = DirectionalLight({ 0,-1,-1 }, { 1,1,1 }, 1.0f);
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

	// Primary method for ray-tracing the scene
	glm::vec3 Scene::TraceRay(const glm::vec3& rayOrigin, const glm::vec3& ray, glm::vec3 rayIntensity, const int& depth)
	{
		// Default return value
		glm::vec3 intensity = glm::vec3(0, 0, 0);

		HitInfo hitInformation;

		// Test whether this ray intersects an object within the scene
		bool intersectsObject = TestIntersection(rayOrigin, ray, hitInformation);

		// No intersection - return default value
		if (!intersectsObject) return intensity;

		// An object was hit - determine its occlusion
		glm::vec3 shadowRay = m_light.DirectionToLight(hitInformation.hitPosition);
		HitInfo occlusionInformation;
		bool isOccluded = TestIntersection(hitInformation.hitPosition, shadowRay, occlusionInformation);

		// Object is in shadow
		if (isOccluded) return intensity;

		// Calculate illumination at the object collision point
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