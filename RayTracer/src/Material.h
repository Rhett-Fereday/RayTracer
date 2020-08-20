#pragma once

#include "CImg.h"
#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API Material
	{
	public:
		Material(const char* textureFile);
		glm::vec3 GetAlbedo(glm::vec2 uv);

	private:
		cimg_library::CImg<unsigned char> m_texture;
	};
}