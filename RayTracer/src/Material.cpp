#include "Material.h"

namespace RayTracer
{
	Material::Material(const char* textureFile)
	{
		m_texture = cimg_library::CImg<unsigned char>(textureFile);
	}

	glm::vec3 Material::GetAlbedo(glm::vec2 uv)
	{
		int xCoord, yCoord;
		yCoord = (1.0f - uv.y) * m_texture.height();
		xCoord = (uv.x) * m_texture.width();

		if (xCoord < 0) xCoord = 0;
		if (yCoord < 0) yCoord = 0;

		if (xCoord > m_texture.width() - 1) xCoord = m_texture.width() - 1;
		if (yCoord > m_texture.height() - 1) yCoord = m_texture.height() - 1;

		glm::vec3 albedo = { m_texture(xCoord, yCoord, 0, 0) / 255.0f, m_texture(xCoord, yCoord, 0, 1) / 255.0f, m_texture(xCoord, yCoord, 0, 2) / 255.0f };

		return albedo;
	}
}