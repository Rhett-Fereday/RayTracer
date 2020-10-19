#include "Scene.h"
#include "RTMath.h"

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

		// An object was hit - determine its lighting
		for (int i = 0; i < m_lights.size(); i++)
		{			
			glm::vec3 shadowRay = m_lights[i]->DirectionToLight(hitInformation.hitPosition);
			HitInfo occlusionInformation;
			bool isOccluded = TestIntersection(hitInformation.hitPosition + 0.001f * shadowRay, shadowRay, occlusionInformation, false);

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
		glm::vec3 diffuseComponent = { 0,0,0 };// clampedIntensity * hitInformation.hitMaterial->albedo * (1 - hitInformation.hitMaterial->reflectiveness);
		glm::vec3 reflectionComponent = { 0,0,0 }; // Must initialize to 0. It does so by default when compiling with VS2017. Not true for VS2019...
		glm::vec3 transmissionComponent = { 0,0,0 };
		glm::vec3 outputColor = { 0,0,0 };

		// Handle transparency
		if (hitInformation.hitMaterial->isTransparent)
		{
			// Calculate the reflection and transmission rays. Use fresnel to determine reflection strength.
			glm::vec3 reflectionRay = glm::normalize(glm::reflect(ray, hitInformation.hitNormal));			
			float reflectionStrength = RTMath::Fresnel(ray, (hitInformation.insideObject) ? -hitInformation.hitNormal : hitInformation.hitNormal, hitInformation.hitMaterial->refractiveIndex);

			if (hitInformation.hitMaterial->reflectionLobeAngle > 0.0f)
			{
				// Calculate random reflection ray in cone about perfect reflection
				glm::vec3 offsetPoint = hitInformation.hitPosition + reflectionRay;
				float maxOffsetDistance = glm::tan(glm::radians(hitInformation.hitMaterial->reflectionLobeAngle / 4.0f));
				float xOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
				float yOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
				float zOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));

				offsetPoint = offsetPoint + glm::vec3(xOffset, yOffset, zOffset);
				reflectionRay = glm::normalize(offsetPoint - hitInformation.hitPosition);
			}

			// Trace the reflection ray
			reflectionComponent = reflectionStrength * TraceRay(hitInformation.hitPosition + 0.001f * reflectionRay, reflectionRay, { 1,1,1 }, depth + 1);

			// Only trace the transmission ray if not internal reflection
			if (reflectionStrength < 1.0f)
			{
				glm::vec3 transmissionRay;
				
				if (hitInformation.insideObject)
				{
					transmissionRay = glm::normalize(glm::refract(ray, hitInformation.hitNormal, hitInformation.hitMaterial->refractiveIndex));
				}
				else
				{
					transmissionRay = glm::normalize(glm::refract(ray, hitInformation.hitNormal, 1.0f / hitInformation.hitMaterial->refractiveIndex));
				}

				if (hitInformation.hitMaterial->transmissionLobeAngle > 0)
				{
					// Calculate random transmission ray in cone about perfect transmission
					glm::vec3 offsetPoint = hitInformation.hitPosition + transmissionRay;
					float maxOffsetDistance = glm::tan(glm::radians(hitInformation.hitMaterial->transmissionLobeAngle / 4.0f));
					float xOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
					float yOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
					float zOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));

					offsetPoint = offsetPoint + glm::vec3(xOffset, yOffset, zOffset);
					transmissionRay = glm::normalize(offsetPoint - hitInformation.hitPosition);
				}

				transmissionComponent = (1.0f - reflectionStrength) * TraceRay(hitInformation.hitPosition + 0.001f * transmissionRay, transmissionRay, { 1,1,1 }, depth + 1);
			}

			outputColor = glm::clamp(reflectionComponent + transmissionComponent, { 0,0,0 }, { 1,1,1 });
		}
		else
		{
			// Handle fully diffuse and partially reflective objects
			diffuseComponent = clampedIntensity * hitInformation.hitMaterial->albedo * (1.0f - hitInformation.hitMaterial->reflectiveness);

			// Only bother with reflection if it matters
			if (hitInformation.hitMaterial->reflectiveness > 0)
			{
				glm::vec3 reflectionRay = glm::normalize(glm::reflect(ray, hitInformation.hitNormal));

				if (hitInformation.hitMaterial->reflectionLobeAngle > 0.0f)
				{
					// Calculate random reflection ray in cone about perfect reflection
					glm::vec3 offsetPoint = hitInformation.hitPosition + reflectionRay;
					float maxOffsetDistance = glm::tan(glm::radians(hitInformation.hitMaterial->reflectionLobeAngle / 4.0f));
					float xOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
					float yOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));
					float zOffset = -maxOffsetDistance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffsetDistance - -maxOffsetDistance)));

					offsetPoint = offsetPoint + glm::vec3(xOffset, yOffset, zOffset);
					reflectionRay = glm::normalize(offsetPoint - hitInformation.hitPosition);
				}

				reflectionComponent = hitInformation.hitMaterial->reflectiveness * TraceRay(hitInformation.hitPosition + 0.001f * reflectionRay, reflectionRay, { 1,1,1 }, depth + 1);
			}

			outputColor = glm::clamp(diffuseComponent + reflectionComponent + hitInformation.hitMaterial->emissiveness, { 0,0,0 }, { 1,1,1 });
		}

		return outputColor;
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