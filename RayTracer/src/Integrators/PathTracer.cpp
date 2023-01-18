#include "PathTracer.h"
#include "../Renderables/Lights/AreaLight.h"

namespace RayTracer
{
	PathTracer::PathTracer(int maxBounces) : Integrator(maxBounces)
	{
	}

	vec3 PathTracer::ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, 
		const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, 
		int pixelSample)
	{
		vec3 L(0), Beta(1);

		Ray r(ray.origin, ray.direction);

		for (int bounce = 0; bounce < maxBounces; bounce++)
		{
			auto interaction = std::make_shared<SurfaceInteraction>();

			if (!scene->Intersects(r, interaction))
			{
				L += Beta * scene->backGroundColor;
				break;
			}

			auto areaLight = interaction->renderable->areaLight;

			if (areaLight)
			{
				if (bounce == 0) L = areaLight->L(*interaction, -r.direction);

				break;
			}

			L += Beta
				* UniformSampleOneLight(interaction, scene, generator, samplesPerLight, pixelSample);

			// Sample new direction assuming all objects are Lambertian
			// Calculate orthonormal basis from the normal
			vec3 w = interaction->geometricNormal;
			vec3 u = glm::normalize(vec3(w.z - w.y, w.x - w.z, w.y - w.x));
			vec3 v = glm::normalize(glm::cross(w, u));

			// Generate two random floats in range [0,1]
			float rand1 = generator->Generate1D();
			float rand2 = generator->Generate1D();

			float root = sqrt(rand2);
			float angle = 2.0f * pi * rand1;

			// Calculate the x, y, and z components of a new random ray assuming a lambertian BRDF
			float a = glm::cos(angle) * root;
			float b = glm::sin(angle) * root;
			float c = sqrt(1.0f - rand2);

			glm::vec3 Wi = glm::normalize(a * u + b * v + c * w);
			r = Ray(interaction->p + interaction->geometricNormal * epsilon, Wi);

			// Lambertian Rho = R/pi
			auto f = interaction->material->GetAlbedo(0, 0) * invPi;
			auto cosWi = glm::max(glm::dot(interaction->geometricNormal, Wi), 0.0f);//glm::abs(glm::dot(interaction->geometricNormal, Wi));
			float pdf = cosWi * invPi;

			if (pdf == 0.0f) break;
			// Update Beta
			Beta *= f * cosWi / pdf;
			//Beta *= (glm::max(glm::dot(interaction->geometricNormal, Wi), 0.0f));
		}

		return L;
	}
}