#include "GaussianBlur.h"
#include "../RTMath.h"

namespace RayTracer
{
	GaussianBlur::GaussianBlur(float sigma)
	{
		m_size = 2 * ceil(sigma) + 1;
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
				float value = RTMath::Gaussian(distance, sigma);
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

	void GaussianBlur::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
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

						blurredColor += (*inputImage)[newY][newX].color * m_kernel[i][j];
					}
				}

				(*outputImage)[y][x].color = blurredColor;
			}
		}
	}
}