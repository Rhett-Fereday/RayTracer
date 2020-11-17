#pragma once

#include "../Core.h"
#include "PostProcess.h"

namespace RayTracer
{
	class RT_API GaussianBlur : public PostProcess
	{
	public:
		GaussianBlur(float sigma);
		void Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height);

	private:
		int m_size;
		float m_sigma;
		float** m_kernel;
	};
}