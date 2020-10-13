#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	// Basic material properties. Emission not currently used
	struct RT_API ConstMaterial
	{
		glm::vec3 albedo = { 1,1,1 };
		glm::vec3 emissiveness = { 0,0,0 };
		float reflectiveness = 0.0f;
		bool isTransparent = false;
		float refractiveIndex = 1.5f;
		float lobeRadius = 0.0f;
	};
}