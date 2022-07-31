#pragma once

#include "../../Core.h"

namespace RayTracer
{
	class RT_API PostProcess
	{
	public:
		virtual vector<vector<vec3>> Apply(const vector<vector<vec3>>& imageBuffer) = 0;
	};
}