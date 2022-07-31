#include "Integrator.h"

namespace RayTracer
{
	void Integrator::Render(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator, float epsilon)
	{
		auto samplesPerPixel = generator->RoundSampleCount(camera->SamplesPerPixel());
		
		vector<int> samplesPerLight(scene->lights.size());

		for (int i = 0; i < scene->lights.size(); i++)
			samplesPerLight[i] = generator->RoundSampleCount(scene->lights[i]->NumSamples());

		for (unsigned int y = 0; y < camera->ImageHeight(); y++)
		{
			for (unsigned int x = 0; x < camera->ImageWidth(); x++)
			{
				vec3 pixelColor(0);

				auto pixelOffsets = generator->Generate2DArray(samplesPerPixel);
				auto lensOffsets = generator->Generate2DArray(samplesPerPixel);

				for (unsigned int s = 0; s < samplesPerPixel; s++)
				{
					Ray ray = camera->GenerateRay(x, y, pixelOffsets[s], lensOffsets[s]);
					pixelColor += ComputeRayContribution(ray, scene, generator, samplesPerLight, s);
				}

				// Set the final value of this pixel
				pixelColor *= 1.0 / static_cast<float>(samplesPerPixel);
				camera->imageBuffer[x][camera->ImageHeight() - y - 1] = pixelColor;
			}
		}
	}

	vec3 Integrator::UniformSampleAllLights(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) const
	{
		vec3 L(0);

		for (int i = 0; i < scene->lights.size(); i++)
		{
			vec3 Ld(0);

			auto samples = generator->Generate2DArray(samplesPerLight[i]);

			for (const auto& sample : samples)
			{
				Ld += EstimateDirectLighting(interaction, scene, scene->lights[i],
					sample);
			}

			L += Ld / static_cast<float>(samplesPerLight[i]);
		}

		// Hack. Currently assuming everything is strictly Lambertian
		auto cosWo = glm::max(glm::dot(interaction->wo, interaction->geometricNormal), 0.0f);
		return L * cosWo;
	}

	vec3 Integrator::EstimateDirectLighting(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<Light> light, const vec2& u) const
	{
		vec3 Ld(0);

		vec3 wi;
		float lightPdf, sampleDistance;

		auto Li = light->SampleLi(*interaction, u, wi, lightPdf, sampleDistance);

		if (lightPdf <= 0 || Li == vec3(0)) return { 0,0,0 };

		auto cosWi = glm::max(glm::dot(wi, interaction->geometricNormal), 0.0f);

		// Assuming Lambertian; Rho = R/pi
		auto f = interaction->material->GetAlbedo(0, 0) *
			invPi * cosWi;

		/*auto f = interaction->material->GetAlbedo(0, 0) *
			invPi;*/

		auto pdf = cosWi * invPi;

		Ld += f * Li / lightPdf;

		if (Ld == vec3(0)) return { 0,0,0 };

		Ray shadowRay(interaction->p + epsilon * interaction->geometricNormal, 
			wi, sampleDistance - 2.0f * epsilon);

		if (scene->Intersects(shadowRay)) return { 0,0,0 };

		return Ld;
	}
}