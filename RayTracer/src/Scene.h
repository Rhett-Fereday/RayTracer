#pragma once

#include <vector>

#include "Camera.h"
#include "Sphere.h"

namespace RayTracer
{
	class Scene
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