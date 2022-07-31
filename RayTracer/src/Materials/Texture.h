#pragma once

#include "../Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API Texture
	{
	public:
		virtual glm::vec3 Sample(float u, float v) const = 0;
	};
}