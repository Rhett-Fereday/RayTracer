#pragma once

#include "../../Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API Lens
	{
	public:
		Lens() = default;
		virtual glm::vec3 GenerateRayOrigin(const vec2& u) = 0;
	};
}