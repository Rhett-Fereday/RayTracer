#include "StratifiedGenerator.h"

namespace RayTracer
{
	vector<float> StratifiedGenerator::Generate1DArray(unsigned int count)
	{
		vector<float> values(count);
		float dx = 1.0f / static_cast<float>(count);

		for (int x = 0; x < count; x++)
		{
			values[x] = static_cast<float>(distribution(mt) + x) * dx;
		}

		return values;
	}

	vector<vec2> StratifiedGenerator::Generate2DArray(unsigned int count)
	{
		vector<vec2> values(count);

		auto dimension = std::sqrt(count);
		float dx = 1.0f / static_cast<float>(dimension);
		float dy = 1.0f / static_cast<float>(dimension);

		int i = 0;

		for (int x = 0; x < dimension; x++)
		{
			for (int y = 0; y < dimension; y++)
			{
				float xVal = static_cast<float>(x + distribution(mt)) * dx;
				float yVal = static_cast<float>(y + distribution(mt)) * dy;

				values[i] = vec2(xVal, yVal);
				i++;
			}
		}

		return values;
	}

	unsigned int StratifiedGenerator::RoundSampleCount(unsigned int count)
	{
		unsigned int roundedSqrt = std::round(std::sqrt(count));

		return roundedSqrt * roundedSqrt;
	}
}