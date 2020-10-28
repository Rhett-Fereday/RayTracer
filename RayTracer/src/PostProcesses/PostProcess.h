#pragma once

#include "../Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API PostProcess
	{
	public:

		virtual glm::vec3** Apply(glm::vec3** inputImage, int width, int height) = 0;
	};
}