#pragma once

#include "../../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API GammaCorrection : public PostProcess
	{
	public:
		vector<vector<vec3>> Apply(const vector<vector<vec3>>& imageBuffer) override;
	};
}