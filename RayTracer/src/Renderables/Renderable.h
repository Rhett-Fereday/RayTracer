#pragma once

#include "../Core.h"
#include "../Ray.h"
#include "Geometry/Geometry.h"
#include "../Materials/Material.h"

namespace RayTracer
{
	class SurfaceInteraction;
	class AreaLight;

	class RT_API Renderable
	{
	public:
		Renderable(shared_ptr<Geometry> geometry, shared_ptr<Material> material, shared_ptr<AreaLight> areaLight = nullptr);
		bool Intersects(const Ray &ray, shared_ptr<SurfaceInteraction> interaction);

	//private:
		shared_ptr<Geometry> geometry;
		shared_ptr<Material> material;
		shared_ptr<AreaLight> areaLight;
	};
}