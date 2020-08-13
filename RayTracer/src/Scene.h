#pragma once

#include <vector>

#include "Camera.h"
#include "Sphere.h"
#include "Core.h"
#include "DirectionalLight.h"

namespace RayTracer
{
	class RT_API Scene
	{
	friend class Camera;

	public:
		Scene(Camera camera);
		void AddSphere(Sphere sphere);
		void RenderScene();
		void SaveScene(const char* filename);

	private:
		Camera m_camera;
		std::vector<Sphere> m_spheres;
		DirectionalLight m_light;

		glm::vec3 TraceRay(const glm::vec3& rayOrigin, const glm::vec3& ray, glm::vec3 rayIntensity, const int& depth);
	};
}