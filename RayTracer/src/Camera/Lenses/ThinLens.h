#pragma once

#include "Lens.h"
#include "../../Core.h"

namespace RayTracer
{
	class RT_API ThinLens : public Lens
	{
	public:
		ThinLens(float lensRadius = 0.01f);
		virtual vec3 GenerateRayOrigin(const vec2& u) override;

	private:
		float lensRadius;
	};
}