//#pragma once
//
//#include "../../Core.h"
//#include "PostProcess.h"
//
//namespace RayTracer
//{
//	class RT_API VGEBF : public PostProcess
//	{
//	public:
//		VGEBF(float sigmaS, float sigmaR, float sigmaN, float sigmaA, float sigmaP);
//		void Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height);
//
//	private:
//		int m_size;
//		float m_sigmaS, m_sigmaR, m_sigmaN, m_sigmaA, m_sigmaP;
//		float** m_kernel;
//	};
//}