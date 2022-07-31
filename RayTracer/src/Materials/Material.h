#pragma once

#include "CImg.h"
#include "../Core.h"
#include "glm/glm.hpp"
#include "Texture.h"

namespace RayTracer
{
	class RT_API Material
	{
	public:
		Material(shared_ptr<Texture> albedoTexture);
		vec3 GetAlbedo(float u, float v);

	private:
		shared_ptr<Texture> albedoTexture;
	};
}