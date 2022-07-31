#include "RayCaster.h"
#include "../Interactions/SurfaceInteraction.h"
#include "../Renderables/Lights/AreaLight.h"

namespace RayTracer
{
	RayCaster::RayCaster() : Integrator()
	{
	}

	vec3 RayCaster::ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, 
		const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, 
		int pixelSample)
	{
		auto interaction = std::make_shared<SurfaceInteraction>();

		if (!scene->Intersects(ray, interaction)) return { 0,0,0 };

		auto areaLight = interaction->renderable->areaLight;
		
		if (areaLight) return areaLight->L(*interaction, -ray.direction);

		return interaction->material->GetAlbedo(0, 0) * 
			UniformSampleAllLights(interaction, scene, generator, samplesPerLight, pixelSample);
	}
}