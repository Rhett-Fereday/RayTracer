#include "Material.h"
#include "ResourceManager.h"

namespace RayTracer
{
	Material::Material(const char* textureFile)
	{
		SetAlbedoTexture(textureFile);
	}

	glm::vec3 Material::GetAlbedo(float u, float v)
	{
		return m_albedoTexture->SampleTexture(u, v);
	}

	void Material::SetAlbedoTexture(const char * textureFile)
	{
		m_albedoTexture = ResourceManager::Instance()->GetTexture(textureFile);
	}
}