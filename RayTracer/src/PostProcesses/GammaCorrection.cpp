#include "GammaCorrection.h"

namespace RayTracer
{
	GammaCorrection::GammaCorrection()
	{
	}

	void GammaCorrection::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
	{
		float correctionFactor = 1.0f / 2.2f;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = (*inputImage)[y][x].color;

				color = glm::pow(color, glm::vec3(correctionFactor));

				(*outputImage)[y][x].color = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
			}
		}
	}
}