#include "BilateralFilter.h"
#include "../RTMath.h"

namespace RayTracer
{
	BilateralFilter::BilateralFilter(int size, float sigmaS, float sigmaR)
	{
		m_size = (size % 2) == 0 ? size + 1 : size;
		m_sigmaS = sigmaS;
		m_sigmaR = sigmaR;

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

	void BilateralFilter::Apply(glm::vec3 ** inputImage, glm::vec3 ** outputImage, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 blurredColor = { 0,0,0 };
				glm::vec3 normalization = { 0,0,0 };

				for (int i = 0; i < m_size; i++)
				{
					for (int j = 0; j < m_size; j++)
					{
						int newX = x + i - int(m_size / 2);
						if ((newX < 0) || (newX >= width)) continue;

						int newY = y + j - int(m_size / 2);
						if ((newY < 0) || (newY >= height)) continue;

						glm::vec3 colorDifference = glm::abs(inputImage[y][x] - inputImage[newY][newX]);
						glm::vec3 rangeWeights = { 0,0,0 };

						rangeWeights.r = RTMath::Gaussian(colorDifference.r, m_sigmaR);
						rangeWeights.g = RTMath::Gaussian(colorDifference.g, m_sigmaR);
						rangeWeights.b = RTMath::Gaussian(colorDifference.b, m_sigmaR);

						glm::vec3 value = m_kernel[i][j] * rangeWeights;
						normalization += value;

						blurredColor += inputImage[newY][newX] * value;
					}
				}

				outputImage[y][x] = blurredColor / normalization;
			}
		}
	}
}