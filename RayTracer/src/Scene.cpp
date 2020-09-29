#include "Scene.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_objects = std::vector<Object*>();
		m_lights = std::vector<Light*>();
		m_recursionLimit = 5;
	}

	void Scene::AddObject(Object* object)
	{
		m_objects.push_back(object);
	}

	void Scene::AddLight(Light* light)
	{
		m_lights.push_back(light);
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

		//return hitInformation.hitMaterial->albedo;

		// An object was hit - determine its lighting
		for (int i = 0; i < m_lights.size(); i++)
		{			
			glm::vec3 shadowRay = m_lights[i]->DirectionToLight(hitInformation.hitPosition);
			HitInfo occlusionInformation;
			bool isOccluded = TestIntersection(hitInformation.hitPosition + 0.001f * shadowRay, shadowRay, occlusionInformation);

			// Object might be occluded for this light
			if (isOccluded)
			{
				// Was the intersection between the ray origin and the light?
				if (occlusionInformation.hitDistance < m_lights[i]->DistanceToLight(hitInformation.hitPosition))
				{
					continue;
				}
			}

			// Calculate illumination for this light at the object collision point
			intensity += m_lights[i]->Illumination(hitInformation.hitPosition, hitInformation.hitNormal, ray);
		}

		// Gotta clamp. Bad things happen when we don't clamp.
		glm::vec3 clampedIntensity = glm::clamp(intensity, { 0,0,0 }, { 1,1,1 });

		glm::vec3 diffuseComponent = clampedIntensity * hitInformation.hitMaterial->albedo * (1 - hitInformation.hitMaterial->reflectiveness);
		
		glm::vec3 reflectionComponent = { 0,0,0 }; // Must initialize to 0. It does so by default when compiling with VS2017. Not true for VS2019...
		if (hitInformation.hitMaterial->reflectiveness > 0)
		{
			glm::vec3 reflectionRay = glm::normalize(glm::reflect(ray, hitInformation.hitNormal));
			reflectionComponent = hitInformation.hitMaterial->reflectiveness * TraceRay(hitInformation.hitPosition + 0.001f * reflectionRay, reflectionRay, { 1,1,1 }, depth + 1);
		}

		// Make sure final result is clamped. No bad juju here
		return glm::clamp(diffuseComponent + reflectionComponent, { 0,0,0 }, { 1,1,1 });
	}

	// Generic object intersection test. Will certainly benefit from some acceleration structure in more complex scenes.
	bool Scene::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation)
	{
		Object* hitObject = nullptr;
		float closest = INFINITY;

		for (int i = 0; i < m_objects.size(); i++)
		{
			HitInfo tempInfo;

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