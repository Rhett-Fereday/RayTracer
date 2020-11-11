#include "GaussianBlur.h"

namespace RayTracer
{
	GaussianBlur::GaussianBlur(int size, float sigma)
	{
		m_size = (size % 2) == 0 ? size + 1 : size;
		m_sigma = sigma;

		int centerX = m_size / 2;
		int centerY = m_size / 2;

		// Compute the blur kernel
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
				float value = ComputeGSigma(distance, sigma);
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

	void GaussianBlur::Apply(glm::vec3 ** inputImage, glm::vec3 ** outputImage, int width, int height)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				glm::vec3 blurredColor = { 0,0,0 };

				for (int i = 0; i < m_size; i++)
				{
					for (int j = 0; j < m_size; j++)
					{
						int newX = x + i - int(m_size / 2);
						if ((newX < 0) || (newX >= width)) continue;

						int newY = y + j - int(m_size / 2);
						if ((newY < 0) || (newY >= height)) continue;

						blurredColor += inputImage[newY][newX] * m_kernel[i][j];
					}
				}

				outputImage[y][x] = blurredColor;
			}
		}
	}

	float GaussianBlur::ComputeGSigma(float distance, float sigma)
	{
		float sigma2 = sigma * sigma;
		float left = 1.0f / (2.0f * 3.14f * sigma2);
		float right = exp(-1.0f * (distance * distance) / (2.0f * sigma2));

		return left * right;
	}
}