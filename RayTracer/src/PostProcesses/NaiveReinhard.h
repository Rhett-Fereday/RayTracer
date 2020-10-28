#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API NaiveReinhard : public PostProcess
	{
	public:
		glm::vec3** Apply(glm::vec3** inputImage, int width, int height);

	};
}