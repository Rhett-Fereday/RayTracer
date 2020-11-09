#include "ModifiedReinhard.h"

namespace RayTracer
{
	void ModifiedReinhard::Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height)
	{
		float maxLuminance = 0.0f;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = inputImage[y][x];

				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });

				if (luminance > maxLuminance) maxLuminance = luminance;
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = inputImage[y][x];
				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });
				float numerator = luminance * (1.0f + (luminance / (maxLuminance * maxLuminance)));
				float newLuminance = numerator / (1.0f + luminance);

				color = color * (newLuminance / luminance);

				outputImage[y][x] = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
			}
		}
	}
}