#pragma once

#include "Core.h"
#include "Object.h"
#include "ConstMaterial.h"
#include "HitInfo.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API Box : public Object
	{
	public:
		Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 dimensions);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		glm::vec3 m_minDimensions, m_maxDimensions;
	};
}