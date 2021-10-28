#pragma once

#include<unordered_map>
#include<memory>
#include<string>

namespace RayTracer
{
	class Texture;

	class TextureManager
	{
	public:
		TextureManager();

		std::weak_ptr<Texture> GetTexture(std::string textureName);

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}