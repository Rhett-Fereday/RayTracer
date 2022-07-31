#pragma once

#include "../Core.h"

namespace RayTracer
{
	class RT_API Interaction
	{
	public:
		vec3 p;
		vec3 wo;
	};
}