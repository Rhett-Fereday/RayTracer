#include "GammaCorrection.h"

namespace RayTracer
{
	vector<vector<vec3>> GammaCorrection::Apply(const vector<vector<vec3>>& imageBuffer)
	{
		vector<vector<vec3>> returnBuffer = imageBuffer;

		int width = imageBuffer.size();
		int height = imageBuffer[0].size();

		float correctionFactor = 1.0f / 2.2f;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 color = returnBuffer[x][y];

				color = glm::pow(color, glm::vec3(correctionFactor));

				returnBuffer[x][y] = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
			}
		}

		return returnBuffer;
	}
}