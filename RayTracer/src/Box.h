#pragma once

#include "Core.h"
#include "Object.h"
#include "ConstMaterial.h"
#include "HitInfo.h"
#include "glm/glm.hpp"
#include "RTMath.h"

namespace RayTracer
{
	class RT_API Box : public Object
	{
	public:
		Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 dimensions);
		Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 minDimensions, glm::vec3 maxDimensions);

	protected:
		bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		RTMath::AABB m_aabb;
	};
}