#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_objects = std::vector<Object*>();
		m_light = DirectionalLight({ -1,-1,-1 }, { 1,1,1 }, 1.0f);
		m_recursionLimit = 3;
	}

	void Scene::AddObject(Object* object)
	{
		m_objects.push_back(object);
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

		if (depth > m_recursionLimit) return intensity;

		HitInfo hitInformation;

		// Test whether this ray intersects an object within the scene
		bool intersectsObject = TestIntersection(rayOrigin, ray, hitInformation);

		// No intersection - return default value
		if (!intersectsObject) return intensity;

		// An object was hit - determine its occlusion
		glm::vec3 shadowRay = m_light.DirectionToLight(hitInformation.hitPosition);
		HitInfo occlusionInformation;
		bool isOccluded = TestIntersection(hitInformation.hitPosition + 0.001f*hitInformation.hitNormal, shadowRay, occlusionInformation);

		// Object is in shadow
		if (isOccluded) return intensity + hitInformation.hitMaterial->emissiveness;

		// Calculate illumination at the object collision point
		intensity += m_light.Illumination(hitInformation.hitPosition, hitInformation.hitNormal, ray);

		glm::vec3 diffuseComponent = hitInformation.hitMaterial->albedo * (1 - hitInformation.hitMaterial->reflectiveness);
		glm::vec3 reflectionRay = glm::reflect(ray, hitInformation.hitNormal);
		glm::vec3 reflectionComponent = hitInformation.hitMaterial->reflectiveness * TraceRay(hitInformation.hitPosition + 0.001f * reflectionRay, reflectionRay, { 1,1,1 }, depth + 1);
		glm::vec3 emissiveComponent = hitInformation.hitMaterial->emissiveness;

		return intensity * (diffuseComponent + reflectionComponent) + emissiveComponent;
	}

	bool Scene::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation)
	{
		Object* hitObject = nullptr;
		float closest = INFINITY;

		HitInfo tempInfo;

		for (int i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i]->Intersects(rayOrigin, ray, tempInfo))
			{
				if (tempInfo.hitDistance < closest)
				{
					closest = tempInfo.hitDistance;
					hitInformation = tempInfo;
					hitObject = m_objects[i];
				}
			}
		}

		if (hitObject == nullptr) return false;

		return true;
	}
}