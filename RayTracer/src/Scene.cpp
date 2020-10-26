#include "Scene.h"
#include "RTMath.h"

namespace RayTracer
{
	Scene::Scene(Camera camera) : m_camera(camera)
	{
		m_objects = std::vector<Object*>();
		m_lights = std::vector<Light*>();
		m_recursionLimit = 20;
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

		// We hit a luminaire
		if (hitInformation.hitMaterial->emissiveStrength > 0)
		{
			//float luminance = glm::dot(hitInformation.hitNormal, -ray) * hitInformation.hitMaterial->emissiveStrength;
			//float luminance = hitInformation.hitMaterial->emissiveStrength / (hitInformation.hitDistance * hitInformation.hitDistance);
			float luminance = hitInformation.hitMaterial->emissiveStrength;

			return luminance * hitInformation.hitMaterial->albedo;
		}

		// Calculate orthonormal basis from the hitNormal
		glm::vec3 w = hitInformation.hitNormal;

		//glm::vec3 planePoint = { 1,1, (glm::dot(w, hitInformation.hitPosition) - w.x - w.y) / w.z };

		//glm::vec3 u = planePoint - hitInformation.hitPosition;

		glm::vec3 u = { w.z - w.y, w.x - w.z, w.y - w.x };

		glm::vec3 v = glm::cross(w, u);

		// Generate two random floats in range [0,1]
		float rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		// Calculate the x, y, and z components of a new random ray assuming a lambertian BRDF
		float a = glm::cos(2.0f * 3.14f * rand1) * sqrt(rand2);
		float b = glm::sin(2.0f * 3.14f * rand1) * sqrt(rand2);
		float c = sqrt(1.0f - rand2);

		glm::vec3 newRay = glm::normalize(a * u + b * v + c * w);

		glm::vec3 lighting = (glm::dot(hitInformation.hitNormal, -ray) * TraceRay(hitInformation.hitPosition + 0.001f * newRay, newRay, { 1,1,1 }, depth + 1)) / (hitInformation.hitDistance * hitInformation.hitDistance);

		return lighting * hitInformation.hitMaterial->albedo;
	}

	// Generic object intersection test. Will certainly benefit from some acceleration structure in more complex scenes.
	bool Scene::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation, bool includeTransparentObjects)
	{
		Object* hitObject = nullptr;
		float closest = INFINITY;

		for (int i = 0; i < m_objects.size(); i++)
		{
			HitInfo tempInfo;

			if (m_objects[i]->Intersects(rayOrigin, ray, tempInfo))
			{
				if (tempInfo.hitMaterial->isTransparent && !includeTransparentObjects) continue;

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