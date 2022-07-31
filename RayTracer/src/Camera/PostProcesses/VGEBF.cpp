//#include "VGEBF.h"
//#include "../RTMath.h"
//
//namespace RayTracer
//{
//	VGEBF::VGEBF(float sigmaS, float sigmaR, float sigmaN, float sigmaA, float sigmaP)
//	{
//		m_size = 2 * ceil(sigmaS) + 1;
//		m_sigmaS = sigmaS;
//		m_sigmaR = sigmaR;
//		m_sigmaN = sigmaN;
//		m_sigmaA = sigmaA;
//		m_sigmaP = sigmaP;
//
//		int centerX = m_size / 2;
//		int centerY = m_size / 2;
//
//		// initialize kernel
//		m_kernel = new float*[m_size];
//		for (int i = 0; i < m_size; i++)
//		{
//			m_kernel[i] = new float[m_size];
//			for (int j = 0; j < m_size; j++)
//			{
//				int diffX = centerX - i;
//				int diffY = centerY - j;
//
//				// store distance for later use
//				m_kernel[i][j] = sqrt((diffX * diffX + diffY * diffY));
//			}
//		}
//	}
//
//	void VGEBF::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
//	{
//		float maxVariance = 0.0f;
//		float meanVariance = 0.0f;
//		float varianceVariance = 0.0f;
//
//		// compute maximum region variance
//		for (int y = 0; y < height; y++)
//		{
//			for (int x = 0; x < width; x++)
//			{
//				float variance = 0.0f;
//				float mean = 0.0f;
//				int n = 0;
//
//				for (int i = 0; i < m_size; i++)
//				{
//					for (int j = 0; j < m_size; j++)
//					{
//						// Ignore out of bounds pixels
//						int newX = x + i - int(m_size / 2);
//						if ((newX < 0) || (newX >= width)) continue;
//
//						int newY = y + j - int(m_size / 2);
//						if ((newY < 0) || (newY >= height)) continue;
//
//						// Compute the mean luminance and the luminance variance in this neighborhood
//						n += 1;
//						float luminance = glm::dot((*inputImage)[newY][newX].color, { 0.2126f, 0.7152f, 0.0722f });
//						mean += luminance;
//						variance += luminance * luminance;
//					}
//				}
//
//				mean /= float(n);
//				variance /= float(n);
//				variance -= (mean * mean);
//				meanVariance += variance;
//				varianceVariance += (variance * variance);
//
//				if (variance > maxVariance) maxVariance = variance;
//			}
//		}
//
//		meanVariance /= (width * height);
//		varianceVariance /= (width * height);
//		varianceVariance = abs(varianceVariance - (meanVariance * meanVariance));
//		float varianceStdDev = sqrt(varianceVariance);
//
//		for (int y = 0; y < height; y++)
//		{
//			for (int x = 0; x < width; x++)
//			{
//				glm::vec3 blurredColor = { 0,0,0 };
//				glm::vec3 normalization = { 0,0,0 };
//
//				float meanLuminance = 0.0f;
//				float luminanceVariance = 0.0f;
//				int n = 0;
//
//				for (int i = 0; i < m_size; i++)
//				{
//					for (int j = 0; j < m_size; j++)
//					{
//						// Ignore out of bounds pixels
//						int newX = x + i - int(m_size / 2);
//						if ((newX < 0) || (newX >= width)) continue;
//
//						int newY = y + j - int(m_size / 2);
//						if ((newY < 0) || (newY >= height)) continue;
//
//						// Compute the mean luminance and the luminance variance in this neighborhood
//						n += 1;
//						float luminance = glm::dot((*inputImage)[newY][newX].color, { 0.2126f, 0.7152f, 0.0722f });
//						meanLuminance += luminance;
//						luminanceVariance += luminance * luminance;
//					}
//				}
//
//				meanLuminance /= float(n);
//				luminanceVariance /= float(n);
//				luminanceVariance -= (meanLuminance * meanLuminance);
//
//				float distance = abs(meanVariance - luminanceVariance);
//				float varianceWeight = RTMath::Gaussian(distance, varianceStdDev);
//
//				// Compute values in the kernel
//				for (int i = 0; i < m_size; i++)
//				{
//					for (int j = 0; j < m_size; j++)
//					{
//						// Ignore out of bounds pixels
//						int newX = x + i - int(m_size / 2);
//						if ((newX < 0) || (newX >= width)) continue;
//
//						int newY = y + j - int(m_size / 2);
//						if ((newY < 0) || (newY >= height)) continue;
//
//						// Calculate spatial weight
//						float distance = m_kernel[i][j];
//						float spatialWeight = RTMath::Gaussian(distance, varianceWeight);
//
//						// Calculate color weights
//						glm::vec3 colorDifference = glm::abs((*inputImage)[y][x].color - (*inputImage)[newY][newX].color);
//						glm::vec3 colorWeights = { 0,0,0 };
//
//						colorWeights.r = RTMath::Gaussian(colorDifference.r, m_sigmaR);
//						colorWeights.g = RTMath::Gaussian(colorDifference.g, m_sigmaR);
//						colorWeights.b = RTMath::Gaussian(colorDifference.b, m_sigmaR);
//
//						// Calculate normal weights
//						float normalDifference = 1.0f - glm::dot((*inputImage)[y][x].normal, (*inputImage)[newY][newX].normal);
//						float normalWeight = 0.0f;
//
//						normalWeight = RTMath::Gaussian(normalDifference, m_sigmaN);
//
//						// Calculate albedo weights
//						glm::vec3 albedoDifference = glm::abs((*inputImage)[y][x].albedo - (*inputImage)[newY][newX].albedo);
//						glm::vec3 albedoWeights = { 0,0,0 };
//
//						albedoWeights.r = RTMath::Gaussian(albedoDifference.r, m_sigmaA);
//						albedoWeights.g = RTMath::Gaussian(albedoDifference.g, m_sigmaA);
//						albedoWeights.b = RTMath::Gaussian(albedoDifference.b, m_sigmaA);
//
//						// Calculate position weights
//						glm::vec3 positionDifference = glm::abs((*inputImage)[y][x].position - (*inputImage)[newY][newX].position);
//						glm::vec3 positionWeights = { 0,0,0 };
//
//						positionWeights.r = RTMath::Gaussian(positionDifference.r, m_sigmaP);
//						positionWeights.g = RTMath::Gaussian(positionDifference.g, m_sigmaP);
//						positionWeights.b = RTMath::Gaussian(positionDifference.b, m_sigmaP);
//
//						// Update normalization factor
//						glm::vec3 value = spatialWeight * colorWeights * normalWeight * albedoWeights * positionWeights;
//						normalization += value;
//
//						blurredColor += (*inputImage)[newY][newX].color * value;
//					}
//				}
//
//				(*outputImage)[y][x].color = blurredColor / normalization;
//			}
//		}
//	}
//}