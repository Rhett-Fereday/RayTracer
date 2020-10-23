#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	// Basic material properties. Emission not currently used
	struct RT_API ConstMaterial
	{
		glm::vec3 albedo = { 1,1,1 };
		glm::vec3 emissiveColor = { 0,0,0 };
		float emissiveStrength = 0.0f;
		float reflectiveness = 0.0f;
		bool isTransparent = false;
		float refractiveIndex = 1.5f;
		float reflectionLobeAngle = 0.0f;
		float transmissionLobeAngle = 0.0f;
	};
}