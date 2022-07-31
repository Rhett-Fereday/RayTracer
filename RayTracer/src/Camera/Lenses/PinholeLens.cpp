#include "PinholeLens.h"

namespace RayTracer
{
	PinholeLens::PinholeLens() : Lens()
	{
	}

	glm::vec3 PinholeLens::GenerateRayOrigin(const vec2& u)
	{
		return { 0,0,0 };
	}
}