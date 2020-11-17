#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	struct GBufferInfo
	{
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec3 albedo;
		glm::vec3 position;
	};
}