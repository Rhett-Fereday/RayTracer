#include "NaiveReinhard.h"

namespace RayTracer
{
	vector<vector<vec3>> NaiveReinhard::Apply(const vector<vector<vec3>>& imageBuffer)
	{
		vector<vector<vec3>> returnBuffer = imageBuffer;

		int width = imageBuffer.size();
		int height = imageBuffer[0].size();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				returnBuffer[y][x] = glm::clamp(imageBuffer[y][x] / (1.0f + imageBuffer[y][x]), { 0,0,0 }, { 1,1,1 });
			}
		}

		return returnBuffer;
	}
}