#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API NaiveReinhard : public PostProcess
	{
	public:
		void Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height);

	};
}