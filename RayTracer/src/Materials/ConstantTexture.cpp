#include "ConstantTexture.h"

namespace RayTracer
{
	ConstantTexture::ConstantTexture(vec3 color) : color(color)
	{
	}

	glm::vec3 ConstantTexture::Sample(float u, float v) const
	{
		return color;
	}
}