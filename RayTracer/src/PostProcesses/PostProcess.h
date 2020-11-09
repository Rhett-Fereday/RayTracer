#pragma once

#include "../Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API PostProcess
	{
	public:
		virtual void Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height) = 0;
	};
}