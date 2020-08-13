#pragma once

#include "glm/glm.hpp"
#include "Core.h"
#include "HitInfo.h"

namespace RayTracer
{
	class RT_API Sphere
	{
	public:
		Sphere(glm::mat4 transform, float radius, glm::vec3 albedo);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo, glm::mat4 cameraTransform);

	private:
		glm::mat4 m_transform;
		float m_radius;
		glm::vec3 m_albedo;
	};
}