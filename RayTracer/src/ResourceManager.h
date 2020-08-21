#pragma once

#include<map>

#include "Core.h"
#include "Texture.h"

namespace RayTracer
{
	class RT_API ResourceManager
	{
	public:
		static ResourceManager* Instance();
		Texture* GetTexture(const char* textureFile);

	private:
		ResourceManager();

		static ResourceManager* m_instance;
		std::map <const char*, Texture*> m_textures;
	};
}