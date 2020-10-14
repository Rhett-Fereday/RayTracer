#pragma once

#include "glm/glm.hpp"
#include "ConstMaterial.h"

namespace RayTracer
{
	struct HitInfo
	{
		glm::vec3 hitPosition;
		glm::vec3 hitNormal;
		ConstMaterial* hitMaterial;
		float hitDistance;
		bool insideObject = false;
	};
}