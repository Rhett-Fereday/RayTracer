#pragma once

#include "Core.h"
#include "CImg.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API Texture
	{
	public:
		Texture(const char* textureFile);
		glm::vec3 SampleTexture(float u, float v);

	private:
		cimg_library::CImg<unsigned char> m_image;
	};
}