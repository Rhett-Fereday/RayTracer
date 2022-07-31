#include "RandomGenerator.h"

namespace RayTracer
{
	RandomGenerator::RandomGenerator() : SampleGenerator()
	{
		distribution = std::uniform_real_distribution<float>(0.0, 1.0);
	}

	float RandomGenerator::Generate1D()
	{
		return distribution(mt);
	}

	vector<float> RandomGenerator::Generate1DArray(unsigned int count)
	{
		vector<float> values(0.0, count);
		for (auto& val : values) val = distribution(mt);

		return values;
	}

	vec2 RandomGenerator::Generate2D()
	{
		return vec2(distribution(mt), distribution(mt));
	}

	vector<vec2> RandomGenerator::Generate2DArray(unsigned int count)
	{
		vector<vec2> values(count);

		for (auto& pair : values) pair = vec2(distribution(mt), distribution(mt));

		return values;
	}
}