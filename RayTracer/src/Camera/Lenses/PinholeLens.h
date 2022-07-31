#pragma once

#include "../../Core.h"
#include "Lens.h"

namespace RayTracer
{
	class RT_API PinholeLens : public Lens
	{
	public:
		PinholeLens();
		virtual glm::vec3 GenerateRayOrigin(const vec2& u) override;
	};
}