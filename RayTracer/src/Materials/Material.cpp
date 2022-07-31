#include "Material.h"

namespace RayTracer
{
	Material::Material(shared_ptr<Texture> albedoTexture) : albedoTexture(albedoTexture)
	{

	}

	vec3 Material::GetAlbedo(float u, float v)
	{
		return albedoTexture->Sample(u, v);
	}
}