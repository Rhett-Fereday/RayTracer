#pragma once

#include <vector>

#include "Camera.h"
#include "Sphere.h"
#include "Core.h"

namespace RayTracer
{
	class RT_API Scene
	{
	public:
		Scene(Camera camera);
		void AddSphere(Sphere sphere);
		void RenderScene();
		void SaveScene(const char* filename);

	private:
		Camera m_camera;
		std::vector<Sphere> m_spheres;
	};
}