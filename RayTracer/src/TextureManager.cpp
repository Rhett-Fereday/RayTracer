#include "TextureManager.h"
#include "Texture.h"

RayTracer::TextureManager::TextureManager()
{
}

std::weak_ptr<RayTracer::Texture> RayTracer::TextureManager::GetTexture(std::string textureName)
{
	auto it = textures.find(textureName);

	if (it != textures.end()) return (*it).second;

	auto texture = std::make_shared<Texture>(textureName.c_str());
	textures[textureName] = texture;

	return texture;
}