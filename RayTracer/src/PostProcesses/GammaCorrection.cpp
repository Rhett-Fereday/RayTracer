#include "GammaCorrection.h"

namespace RayTracer
{
	GammaCorrection::GammaCorrection()
	{
	}

	void GammaCorrection::Apply(glm::vec3 ** inputImage, glm::vec3 ** outputImage, int width, int height)
	{
		float correctionFactor = 1.0f / 2.2f;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = inputImage[y][x];

				color = glm::pow(color, glm::vec3(correctionFactor));

				outputImage[y][x] = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
			}
		}
	}
}