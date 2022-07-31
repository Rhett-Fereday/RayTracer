#pragma once

#include "Interaction.h"
#include "../Materials/Material.h"

namespace RayTracer
{
	class Renderable;

	class RT_API SurfaceInteraction : public Interaction
	{
	public:
		SurfaceInteraction() = default;
		vec3 geometricNormal;
		vec3 shadingNormal;
		vec2 uv;
		shared_ptr<Renderable> renderable;
		shared_ptr<Material> material;
	};
}