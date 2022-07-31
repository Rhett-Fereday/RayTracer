#include "Renderable.h"
#include "Lights/AreaLight.h"
#include "../Interactions/SurfaceInteraction.h"

namespace RayTracer
{
	Renderable::Renderable(shared_ptr<Geometry> geometry, shared_ptr<Material> material, shared_ptr<AreaLight> areaLight)
		: geometry(geometry), material(material), areaLight(areaLight)
	{
	}

	bool Renderable::Intersects(const Ray &ray, shared_ptr<SurfaceInteraction> interaction)
	{
		float tHit;

		if (!geometry->Intersects(ray, tHit, interaction)) return false;

		ray.tMax = tHit;
		interaction->material = material;
		interaction->renderable = std::make_shared<Renderable>(*this);
		interaction->shadingNormal = interaction->geometricNormal;

		return true;
	}
}