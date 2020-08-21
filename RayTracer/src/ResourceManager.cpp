#include "ResourceManager.h"

namespace RayTracer
{
	ResourceManager* ResourceManager::m_instance = nullptr;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager* ResourceManager::Instance()
	{
		if (ResourceManager::m_instance == nullptr) ResourceManager::m_instance = new ResourceManager();

		return ResourceManager::m_instance;
	}

	Texture* ResourceManager::GetTexture(const char * textureFile)
	{
		std::map<const char *, Texture*>::iterator it;

		it = m_textures.find(textureFile);

		if (it != m_textures.end()) return (*it).second;

		Texture* texture = m_textures[textureFile] = new Texture(textureFile);

		return texture;
	}
}