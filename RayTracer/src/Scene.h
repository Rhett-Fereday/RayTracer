#pragma once

#include <vector>

#include "Camera.h"
#include "Objects/Object.h"
#include "Core.h"
#include "Lights/Light.h"

namespace RayTracer
{
	class RT_API Scene
	{
	friend class Camera;

	public:
		Scene(Camera camera);
		void AddObject(Object* object);
		void AddLight(Light* light);
		void RenderScene();
		void SaveScene(const char* filename);

	private:
		Camera m_camera;
		std::vector<Object*> m_objects;
		std::vector<Light*> m_lights;
		int m_recursionLimit;

		glm::vec3 TraceRay(const glm::vec3& rayOrigin, const glm::vec3& ray, glm::vec3 rayIntensity, const int& depth);
		bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& ray, HitInfo& hitInformation);
	};
}