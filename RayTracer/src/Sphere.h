#pragma once

#include "glm/glm.hpp"
#include "Core.h"
#include "Object.h"
#include "ConstMaterial.h"
#include "HitInfo.h"

namespace RayTracer
{
	class RT_API Sphere : public Object
	{
	public:
		Sphere(glm::mat4 transform, ConstMaterial* material, float radius);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		float m_radius;
		glm::vec3 m_center;
	};
}