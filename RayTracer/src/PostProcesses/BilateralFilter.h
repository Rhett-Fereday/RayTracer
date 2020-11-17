#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API BilateralFilter : public PostProcess
	{
	public:
		BilateralFilter(float sigmaS, float sigmaR);
		void Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height);

	private:
		int m_size;
		float m_sigmaS, m_sigmaR;
		float** m_kernel;
	};
}