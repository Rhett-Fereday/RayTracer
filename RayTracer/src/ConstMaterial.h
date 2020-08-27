#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	struct RT_API ConstMaterial
	{
		glm::vec3 albedo;
	};
}