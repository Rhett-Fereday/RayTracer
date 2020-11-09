#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	// Basic material properties. Emission not currently used
	struct RT_API ConstMaterial
	{
		glm::vec3 albedo = { 1,1,1 };
		float emissiveStrength = 0.0f;
		bool isTransparent = false;
		bool isMetal = false;
		float refractiveIndex = 1.5f;
		float roughness = 1.0f;
	};
}