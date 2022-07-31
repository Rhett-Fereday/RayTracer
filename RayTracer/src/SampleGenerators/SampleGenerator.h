#pragma once

#include "../Core.h"
#include <random>

namespace RayTracer
{
	class RT_API SampleGenerator
	{
	public:
		SampleGenerator()
		{
			mt = std::mt19937(rd());
		}

		virtual float Generate1D() = 0;
		virtual vector<float> Generate1DArray(unsigned int count) = 0;

		virtual vec2 Generate2D() = 0;
		virtual vector<vec2> Generate2DArray(unsigned int count) = 0;

		inline virtual unsigned int RoundSampleCount(unsigned int count) { return count; };

	protected:

		std::random_device rd;
		std::mt19937 mt;
	};
}