#pragma once

#include "glm/glm.hpp"
#include "../Core.h"
#include "Object.h"
#include "../ConstMaterial.h"
#include "../HitInfo.h"

namespace RayTracer
{
	class RT_API Sphere : public Object
	{
	public:
		Sphere(glm::mat4 transform, ConstMaterial* material);

	protected:
		bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);
	};
}