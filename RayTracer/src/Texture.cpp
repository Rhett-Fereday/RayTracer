#include "Texture.h"

namespace RayTracer
{
	Texture::Texture(const char * textureFile)
	{
		m_image = cimg_library::CImg<unsigned char>(textureFile);
	}

	glm::vec3 Texture::SampleTexture(float u, float v)
	{
		int xCoord, yCoord;
		
		xCoord = u * m_image.width();
		yCoord = (1.0f - v) * m_image.height();

		if (xCoord < 0) xCoord = 0;
		if (yCoord < 0) yCoord = 0;

		if (xCoord > m_image.width() - 1) xCoord = m_image.width() - 1;
		if (yCoord > m_image.height() - 1) yCoord = m_image.height() - 1;

		glm::vec3 sampleColor = { m_image(xCoord, yCoord, 0, 0) / 255.0f, m_image(xCoord, yCoord, 0, 1) / 255.0f, m_image(xCoord, yCoord, 0, 2) / 255.0f };

		return sampleColor;
	}
}