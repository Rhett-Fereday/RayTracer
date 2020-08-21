#pragma once

#include "CImg.h"
#include "Core.h"
#include "glm/glm.hpp"
#include "Texture.h"

namespace RayTracer
{
	class RT_API Material
	{
	public:
		Material(const char* textureFile);
		glm::vec3 GetAlbedo(float u, float v);
		void SetAlbedoTexture(const char* textureFile);

	private:
		Texture* m_albedoTexture;
		//Texture* m_normalTexture;
	};
}