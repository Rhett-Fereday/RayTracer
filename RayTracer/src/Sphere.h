#pragma once

#include "glm/glm.hpp"
#include "Core.h"
#include "HitInfo.h"
#include "Material.h"

namespace RayTracer
{
	class RT_API Sphere
	{
	public:
		Sphere(glm::mat4 transform, float radius, Material material);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		glm::mat4 m_transform;
		glm::mat4 m_inverseTransform;
		float m_radius;
		glm::vec3 m_albedo;
		Material m_material;
	};
}