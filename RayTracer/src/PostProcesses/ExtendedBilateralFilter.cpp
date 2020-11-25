#include "ExtendedBilateralFilter.h"
#include "../RTMath.h"

namespace RayTracer
{
	ExtendedBilateralFilter::ExtendedBilateralFilter(float sigmaS, float sigmaR, float sigmaN, float sigmaA, float sigmaP)
	{
		m_size = 2 * ceil(sigmaS) + 1;
		m_sigmaS = sigmaS;
		m_sigmaR = sigmaR;
		m_sigmaN = sigmaN;
		m_sigmaA = sigmaA;
		m_sigmaP = sigmaP;

		int centerX = m_size / 2;
		int centerY = m_size / 2;

		// Compute the standard Gaussian portion of the blur kernel
		float normalization = 0.0f;
		m_kernel = new float*[m_size];
		for (int i = 0; i < m_size; i++)
		{
			m_kernel[i] = new float[m_size];
			for (int j = 0; j < m_size; j++)
			{
				int diffX = centerX - i;
				int diffY = centerY - j;

				float distance = sqrt((diffX * diffX + diffY * diffY));
				float value = RTMath::Gaussian(distance, m_sigmaS);
				normalization += value;
				m_kernel[i][j] = value;
			}
		}

		// Normalize the elements of the kernel
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				m_kernel[i][j] = m_kernel[i][j] / normalization;
			}
		}
	}

	void ExtendedBilateralFilter::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 blurredColor = { 0,0,0 };
				glm::vec3 normalization = { 0,0,0 };

				// Apply the kernel
				for (int i = 0; i < m_size; i++)
				{
					for (int j = 0; j < m_size; j++)
					{
						// Ignore out of bounds pixels
						int newX = x + i - int(m_size / 2);
						if ((newX < 0) || (newX >= width)) continue;

						int newY = y + j - int(m_size / 2);
						if ((newY < 0) || (newY >= height)) continue;

						// Calculate color weights
						glm::vec3 colorDifference = glm::abs((*inputImage)[y][x].color - (*inputImage)[newY][newX].color);
						glm::vec3 colorWeights = { 0,0,0 };

						colorWeights.r = RTMath::Gaussian(colorDifference.r, m_sigmaR);
						colorWeights.g = RTMath::Gaussian(colorDifference.g, m_sigmaR);
						colorWeights.b = RTMath::Gaussian(colorDifference.b, m_sigmaR);

						// Calculate normal weights
						float normalDifference = 1.0f - glm::dot((*inputImage)[y][x].normal, (*inputImage)[newY][newX].normal);
						float normalWeight = 0.0f;

						normalWeight = RTMath::Gaussian(normalDifference, m_sigmaN);

						// Calculate albedo weights
						glm::vec3 albedoDifference = glm::abs((*inputImage)[y][x].albedo - (*inputImage)[newY][newX].albedo);
						glm::vec3 albedoWeights = { 0,0,0 };

						albedoWeights.r = RTMath::Gaussian(albedoDifference.r, m_sigmaA);
						albedoWeights.g = RTMath::Gaussian(albedoDifference.g, m_sigmaA);
						albedoWeights.b = RTMath::Gaussian(albedoDifference.b, m_sigmaA);

						// Calculate position weights
						glm::vec3 positionDifference = glm::abs((*inputImage)[y][x].position - (*inputImage)[newY][newX].position);
						glm::vec3 positionWeights = { 0,0,0 };

						positionWeights.r = RTMath::Gaussian(positionDifference.r, m_sigmaP);
						positionWeights.g = RTMath::Gaussian(positionDifference.g, m_sigmaP);
						positionWeights.b = RTMath::Gaussian(positionDifference.b, m_sigmaP);

						// Update normalization factor
						glm::vec3 value = m_kernel[i][j] * colorWeights * normalWeight * albedoWeights * positionWeights;
						normalization += value;

						blurredColor += (*inputImage)[newY][newX].color * value;
					}
				}

				(*outputImage)[y][x].color = blurredColor / normalization;
			}
		}
	}
}