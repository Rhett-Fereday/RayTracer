#include "ModifiedReinhard.h"

namespace RayTracer
{
	vector<vector<vec3>> ModifiedReinhard::Apply(const vector<vector<vec3>>& imageBuffer)
	{
		float maxLuminance = 0.0f;
		auto returnBuffer = imageBuffer;

		int width = imageBuffer.size();
		int height = imageBuffer[0].size();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = imageBuffer[x][y];

				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });

				if (luminance > maxLuminance) maxLuminance = luminance;
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = imageBuffer[x][y];
				float luminance = glm::dot(color, { 0.2126f, 0.7152f, 0.0722f });
				float numerator = luminance * (1.0f + (luminance / (maxLuminance * maxLuminance)));
				float newLuminance = numerator / (1.0f + luminance);

				color = color * (newLuminance / luminance);

				returnBuffer[x][y] = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
			}
		}

		return returnBuffer;
	}
}