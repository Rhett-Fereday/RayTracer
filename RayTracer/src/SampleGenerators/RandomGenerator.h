#pragma once

#include "SampleGenerator.h"

namespace RayTracer
{
	class RT_API RandomGenerator : public SampleGenerator
	{
	public:
		RandomGenerator();

		virtual float Generate1D() override;

		virtual vector<float> Generate1DArray(unsigned int count) override;

		virtual vec2 Generate2D() override;

		virtual vector<vec2> Generate2DArray(unsigned int count) override;

	protected:
		std::uniform_real_distribution<float> distribution;
	};
}