#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	// Basic material properties. Emission not currently used
	struct RT_API ConstMaterial
	{
		glm::vec3 albedo;
		glm::vec3 emissiveness;
		float reflectiveness;
		bool isTransparent;
		float refractiveIndex;
	};
}