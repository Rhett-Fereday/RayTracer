#include "NaiveReinhard.h"

namespace RayTracer
{
	void NaiveReinhard::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				(*outputImage)[y][x].color = glm::clamp((*inputImage)[y][x].color / (1.0f + (*inputImage)[y][x].color), { 0,0,0 }, { 1,1,1 });
			}
		}
	}
}