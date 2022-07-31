#pragma once

#include "Texture.h"

namespace RayTracer
{
	class RT_API ConstantTexture : public Texture
	{
	public:
		ConstantTexture(vec3 color);

		virtual glm::vec3 Sample(float u, float v) const override;

	private:
		vec3 color;
	};
}