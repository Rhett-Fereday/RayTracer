#include "NaiveReinhard.h"

namespace RayTracer
{
	glm::vec3 ** RayTracer::NaiveReinhard::Apply(glm::vec3 ** inputImage, int width, int height)
	{
		//glm::vec3** outputImage = new glm::vec3[width * height];
		glm::vec3** outputImage = inputImage;

		/*for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				outputImage[y][x] = inputImage[y][x] / (1.0f + inputImage[y][x]);
			}
		}*/

		float maxLuminance = 0.0f;
		float avgLuminance = 0.0f;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = inputImage[y][x];
				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });
				avgLuminance += luminance;

				if (luminance > maxLuminance) maxLuminance = luminance;
			}
		}

		avgLuminance = avgLuminance / float(width * height);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				/*glm::vec3 color = inputImage[y][x];
				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });
				float numerator = luminance * (1.0f + (luminance / (maxLuminance * maxLuminance)));
				float newLuminance = numerator / (1.0f + luminance);*/
				glm::vec3 color = inputImage[y][x];
				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });
				float numerator = luminance * (1.0f + (luminance / (maxLuminance * maxLuminance)));
				float newLuminance = numerator / (1.0f + luminance);

				outputImage[y][x] = color * (newLuminance / luminance);
			}
		}

		return outputImage;
	}
}