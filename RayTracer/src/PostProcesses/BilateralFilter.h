#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API BilateralFilter : public PostProcess
	{
	public:
		BilateralFilter(int size, float sigmaS, float sigmaR);
		void Apply(glm::vec3** inputImage, glm::vec3** outputImage, int width, int height);

	private:
		int m_size;
		float m_sigmaS, m_sigmaR;
		float** m_kernel;
	};
}