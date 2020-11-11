#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API GaussianBlur : public PostProcess
	{
	public:
		GaussianBlur(int size, float sigma);
		void Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height);

	private:
		int m_size;
		float m_sigma;
		float** m_kernel;

		float ComputeGSigma(float distance, float sigma);
	};
}