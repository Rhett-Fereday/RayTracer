#pragma once

#include "../Core.h"
#include "glm/glm.hpp"
#include "../GBufferInfo.h"
#include <vector>

namespace RayTracer
{
	class RT_API PostProcess
	{
	public:
		virtual void Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height) = 0;
	};
}