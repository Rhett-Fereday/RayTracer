#pragma once

#include "Core.h"

namespace RayTracer
{
	class RT_API Ray
	{
	public:
		Ray() = default;
		Ray(vec3 origin, vec3 direction, float tMax = infinity) 
			: origin(origin), direction(direction), tMax(tMax) { }

		vec3 origin, direction;
		mutable float tMax;
	};
}