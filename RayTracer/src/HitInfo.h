#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	struct HitInfo
	{
		glm::vec3 hitPosition;
		glm::vec3 hitNormal;
		glm::vec3 hitColor;
	};
}