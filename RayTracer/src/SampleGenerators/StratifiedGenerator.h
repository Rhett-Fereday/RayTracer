#pragma once

#include "RandomGenerator.h"

namespace RayTracer
{
	class RT_API StratifiedGenerator : public RandomGenerator
	{
	public:
		virtual vector<float> Generate1DArray(unsigned int count) override;

		virtual vector<vec2> Generate2DArray(unsigned int count) override;

		virtual unsigned int RoundSampleCount(unsigned int count) override;
	};
}