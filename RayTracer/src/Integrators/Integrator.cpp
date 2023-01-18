#include "Integrator.h"

namespace RayTracer
{
	void Integrator::Render(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator, int numThreads)
	{
		//auto samplesPerPixel = generator->RoundSampleCount(camera->SamplesPerPixel());
		//
		//vector<int> samplesPerLight(scene->lights.size());

		//for (int i = 0; i < scene->lights.size(); i++)
		//	samplesPerLight[i] = generator->RoundSampleCount(scene->lights[i]->NumSamples());

		//for (unsigned int y = 0; y < camera->ImageHeight(); y++)
		//{
		//	for (unsigned int x = 0; x < camera->ImageWidth(); x++)
		//	{
		//		vec3 pixelColor(0);

		//		auto pixelOffsets = generator->Generate2DArray(samplesPerPixel);
		//		auto lensOffsets = generator->Generate2DArray(samplesPerPixel);

		//		for (unsigned int s = 0; s < samplesPerPixel; s++)
		//		{
		//			Ray ray = camera->GenerateRay(x, y, pixelOffsets[s], lensOffsets[s]);
		//			pixelColor += ComputeRayContribution(ray, scene, generator, samplesPerLight, s);
		//		}

		//		// Set the final value of this pixel
		//		pixelColor *= 1.0 / static_cast<float>(samplesPerPixel);
		//		camera->imageBuffer[x][camera->ImageHeight() - y - 1] = pixelColor;
		//	}
		//}

		int numTilesWide = std::ceil(camera->ImageWidth() / tileDim);
		int numTilesHigh = std::ceil(camera->ImageHeight() / tileDim);
		
		for (int i = 0; i < numTilesWide; i++)
		{
			for (int j = 0; j < numTilesHigh; j++)
			{
				RenderTile tile;
				tile.startX = i * tileDim;
				tile.startY = j * tileDim;

				renderQueue.push(tile);
			}
		}

		vector<std::thread> threads(numThreads);

		for (auto& thread : threads) 
			thread = std::thread(&Integrator::ThreadRender,this, scene, camera, generator);

		ThreadRender(scene, camera, generator);

		for (auto& thread : threads) thread.join();
	}

	void Integrator::ThreadRender(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator)
	{
		auto samplesPerPixel = generator->RoundSampleCount(camera->SamplesPerPixel());
		
		vector<int> samplesPerLight(scene->lights.size());

		for (int i = 0; i < scene->lights.size(); i++)
			samplesPerLight[i] = generator->RoundSampleCount(scene->lights[i]->NumSamples());

		while (!renderQueue.empty())
		{
			renderMutex.lock();

			auto tile = renderQueue.front();
			renderQueue.pop();

			renderMutex.unlock();

			auto yMax = tile.startY + tileDim;
			auto xMax = tile.startX + tileDim;

			if (yMax > camera->ImageHeight()) yMax = camera->ImageHeight();
			if (xMax > camera->ImageWidth()) xMax = camera->ImageWidth();

			for (unsigned int y = tile.startY; y < yMax; y++)
			{
				for (unsigned int x = tile.startX; x < xMax; x++)
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

					renderMutex.lock();

					camera->imageBuffer[x][camera->ImageHeight() - y - 1] = pixelColor;

					renderMutex.unlock();
				}
			}
		}
	}

	vec3 Integrator::UniformSampleOneLight(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) const
	{
		/*vec3 L(0);

		int nLights = scene->lights.size();
		float lightPdf = 1.0f / static_cast<float>(nLights);

		auto lightNum = std::min((int)(generator->Generate1D() * nLights), nLights - 1);

		auto samples = generator->Generate2DArray(samplesPerLight[lightNum]);

		for (const auto& sample : samples)
		{
			L += EstimateDirectLighting(interaction, scene, scene->lights[lightNum],
				sample);
		}

		L /= static_cast<float>(samplesPerLight[lightNum]);

		return L / lightPdf;*/

		// Perform a weighted random sampling of the light sources based on total power / distance^2
		float normalizer = 0.0f;
		vector<std::pair<float, int>> lightPdfs;

		// Compute the pdf for each light
		for (int light = 0; light < scene->lights.size(); light++)
		{
			auto distance = glm::distance(scene->lights[light]->Position(), interaction->p);
			auto value = scene->lights[light]->Power() / (distance * distance);
			normalizer += value;
			lightPdfs.push_back(std::make_pair(value, light));
		}

		// Normalize the pdfs
		normalizer = 1.0f / normalizer;
		for (auto& pair : lightPdfs)
		{
			pair.first *= normalizer;
		}

		// Sort based on the pdf and then sample
		std::sort(lightPdfs.begin(), lightPdfs.end(), [](const auto a, const auto b) {
			return a.first > b.first;
		});

		auto val = generator->Generate1D();
		auto accumulator = 0.0f;
		int sampleIndex = lightPdfs[lightPdfs.size() - 1].second;
		float samplePdf = lightPdfs[lightPdfs.size() - 1].first;

		for (const auto& pair : lightPdfs)
		{
			accumulator += pair.first;

			if (accumulator >= val)
			{
				samplePdf = pair.first;
				sampleIndex = pair.second;
				break;
			}
		}

		vec3 L(0);
		auto samples = generator->Generate2DArray(samplesPerLight[sampleIndex]);

		for (const auto& sample : samples)
		{
			L += EstimateDirectLighting(interaction, scene, scene->lights[sampleIndex],
				sample);
		}

		L /= static_cast<float>(samplesPerLight[sampleIndex]);

		return L / samplePdf;
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

		return L;
	}

	vec3 Integrator::EstimateDirectLighting(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<Light> light, const vec2& u) const
	{
		vec3 Ld(0);

		vec3 wi;
		float lightPdf, sampleDistance;

		auto Li = light->SampleLi(*interaction, u, wi, lightPdf, sampleDistance);

		if (lightPdf <= 0 || Li == vec3(0)) return { 0,0,0 };

		wi = glm::normalize(wi);
		auto cosWi = glm::abs(glm::dot(wi, glm::normalize(interaction->geometricNormal)));

		// Assuming Lambertian; Rho = R/pi
		auto f = interaction->material->GetAlbedo(0, 0) *
			invPi;

		auto pdf = cosWi * invPi;
		//f = f / pdf;

		auto beta = f * cosWi;

		 float weightf = 1.0 * lightPdf, g = 1.0 * pdf;

		 float weight = 1.0f;// (weightf * weightf) / (weightf * weightf + g * g);

		Ld += beta * weight * Li / lightPdf;

		if (Ld == vec3(0)) return { 0,0,0 };

		Ray shadowRay(interaction->p + epsilon * interaction->geometricNormal, 
			wi, sampleDistance - 2.0f * epsilon);

		if (scene->Intersects(shadowRay)) return { 0,0,0 };

		return Ld;
	}
}