#include "ThinLens.h"
#include "../../RTMath.h"

namespace RayTracer
{
	ThinLens::ThinLens(float lensRadius) : lensRadius(lensRadius)
	{
	}

	vec3 ThinLens::GenerateRayOrigin(const vec2& u)
	{
		vec3 origin = vec3(lensRadius * RTMath::ConcentricSampleDisk(u), 0);

		return origin;
	}
}