#include "NaiveReinhard.h"

namespace RayTracer
{
	void NaiveReinhard::Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				outputImage[y][x] = glm::clamp(inputImage[y][x] / (1.0f + inputImage[y][x]), { 0,0,0 }, { 1,1,1 });
			}
		}
	}
}