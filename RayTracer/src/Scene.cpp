#include "Scene.h"
#include "RTMath.h"

namespace RayTracer
{
	Scene::Scene(Camera* camera)
	{
		m_camera = camera;
		m_objects = std::vector<Object*>();
		m_lights = std::vector<Light*>();
		m_recursionLimit = 16;
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
		m_camera->Render(this);
	}
	
	void Scene::SaveScene(const char* filename)
	{
		m_camera->GetRenderTarget().save(filename);
	}

	// Primary method for ray-tracing the scene
	glm::vec3 Scene::TraceRay(const glm::vec3& rayOrigin, const glm::vec3& ray, glm::vec3 rayIntensity, const int& depth)
	{
		// Default return value
		glm::vec3 intensity = glm::vec3(0, 0, 0);

		if (depth > m_recursionLimit) return intensity;

		HitInfo hitInformation;
		bool intersectsObject;

		if (depth > 1)
		{
			// Test whether this ray intersects an object within the scene
			intersectsObject = TestIntersection(rayOrigin, ray, hitInformation);
		}
		else
		{
			intersectsObject = TestIntersection(rayOrigin, ray, hitInformation, true, true);
		}

		// No intersection - return default value
		if (!intersectsObject) return intensity;		

		if (hitInformation.hitMaterial->emissiveStrength > 0)
		{
			if (depth > 1) return intensity;
			else
			{
				float incidence = std::max(0.0f, glm::dot(hitInformation.hitNormal, -ray));

				return hitInformation.hitMaterial->emissiveStrength * hitInformation.hitMaterial->albedo;
			}
		}

		glm::vec3 indirectComponent = { 0,0,0 };
		glm::vec3 directComponent = { 0,0,0 };

		// Calculate direct component
		for (int i = 0; i < m_lights.size(); i++)
		{
			glm::vec3 occlusionRay;
			float pdf;
			float sampleDistance;
			glm::vec3 irradiance = m_lights[i]->SampleRadiance(hitInformation.hitPosition, hitInformation.hitNormal, occlusionRay, pdf, sampleDistance);
			
			HitInfo occlusionInfo;
			bool occluded = TestIntersection(hitInformation.hitPosition + 0.0001f * occlusionRay, occlusionRay, occlusionInfo, false);
			
			if (occluded)
			{
				if (occlusionInfo.hitDistance < sampleDistance)
				{
					continue;
				}
			}

			directComponent += irradiance / pdf;
		}

		//Calculate the indirect component
		{
			// Calculate orthonormal basis from the hitNormal
			glm::vec3 w = hitInformation.hitNormal;
			glm::vec3 u = { w.z - w.y, w.x - w.z, w.y - w.x };
			glm::vec3 v = glm::cross(w, u);

			// Generate two random floats in range [0,1]
			float rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			// Calculate the x, y, and z components of a new random ray assuming a lambertian BRDF
			float a = glm::cos(2.0f * 3.14f * rand1) * sqrt(rand2);
			float b = glm::sin(2.0f * 3.14f * rand1) * sqrt(rand2);
			float c = sqrt(1.0f - rand2);

			glm::vec3 indirectSample = glm::normalize(a * u + b * v + c * w);
			float cos_theta = std::max(0.0f, glm::dot(hitInformation.hitNormal, indirectSample));
			indirectComponent = TraceRay(hitInformation.hitPosition + 0.001f * indirectSample, indirectSample, { 1,1,1 }, depth + 1);
		}

		float cos_theta_prime = std::max(0.0f, glm::dot(hitInformation.hitNormal, glm::normalize(-ray)));
		glm::vec3 totalLighting = directComponent + indirectComponent;

		return totalLighting * hitInformation.hitMaterial->albedo;
	}

	// Generic object intersection test. Will certainly benefit from some acceleration structure in more complex scenes.
	bool Scene::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation, bool includeTransparentObjects, bool includeEmissiveObjects)
	{
		Object* hitObject = nullptr;
		float closest = INFINITY;

		for (int i = 0; i < m_objects.size(); i++)
		{
			HitInfo tempInfo;

			if (m_objects[i]->Intersects(rayOrigin, ray, tempInfo))
			{
				if (tempInfo.hitMaterial->isTransparent && !includeTransparentObjects) continue;
				if ((tempInfo.hitMaterial->emissiveStrength > 0) && !includeEmissiveObjects) continue;

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