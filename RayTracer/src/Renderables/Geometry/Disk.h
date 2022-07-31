#pragma once

#include "Geometry.h"
#include "../../Core.h"

namespace RayTracer
{
	class RT_API Disk : public Geometry
	{
	public:
		Disk(glm::mat4 transform, float radius = 1.0f);

		virtual SurfaceInteraction Sample(const vec2& u) const override;

		virtual float Area() const override;

	protected:
		virtual bool TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const override;

	private:
		const float radius;
	};
}