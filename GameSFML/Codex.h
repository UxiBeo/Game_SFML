#pragma once

#include "entt/resource/cache.hpp"

template<typename Resource>
class Codex
{
public:
	static const Resource& Retrieve(entt::hashed_string filename)
	{
		return GetCodex().GetResource(filename);
	}
	static Resource& RetrieveNonConst(entt::hashed_string filename)
	{
		return GetCodex().GetResourceNonConst(filename);
	}
	static void LoadFromFile(entt::hashed_string filename)
	{
		GetCodex().Load(filename);
	}
private:
	const Resource& GetResource(entt::hashed_string filename)
	{
		if (cache.contains(filename))
		{
			return cache.handle(filename).get();
		}

		cache.load<Resource::Loader>(filename, filename);
		return cache.handle(filename).get();
	}
	void Load(entt::hashed_string filename)
	{
		if (cache.contains(filename))
			return;

		cache.load<Resource::Loader>(filename, filename);
	}
	Resource& GetResourceNonConst(entt::hashed_string filename)
	{
		if (cache.contains(filename))
		{
			return cache.handle(filename).get();
		}

		cache.load<Resource::Loader>(filename, filename);
		return cache.handle(filename).get();
	}
	static Codex& GetCodex()
	{
		static Codex codex;
		return codex;
	}
private:
	entt::cache<Resource> cache;
};


//class Codex
//{
//public:
//	const sf::Texture& GetTexture(entt::hashed_string filename)
//	{
//		if (textureCache.contains(filename))
//		{
//			return textureCache.handle(filename).get();
//		}
//
//		textureCache.load<TextureLoader>(filename, filename);
//		return textureCache.handle(filename).get();
//	}
//	const AnimationResource& GetAnimation(entt::hashed_string filename)
//	{
//		if (animationCache.contains(filename))
//		{
//			return animationCache.handle(filename).get();
//		}
//		const nlohmann::json Json = GetJson(filename);
//
//		std::string imageName{ "Data\\Images\\" };
//		imageName += Json["imageName"].get<std::string>();
//		entt::hashed_string texturePath{ imageName.c_str() };
//
//		animationCache.load<AnimationLoader>(filename, Json, GetTexture(texturePath));
//		return animationCache.handle(filename).get();
//	}
//	const MapResource& GetMapResource(entt::hashed_string filename)
//	{
//		if (mapCache.contains(filename))
//		{
//			return mapCache.handle(filename).get();
//		}
//		const nlohmann::json Json = GetJson(filename);
//
//		std::string imageName{ "Data\\Images\\Tile\\" };
//		for (auto& tileset : Json["tilesets"])
//		{
//			imageName += tileset["image"].get<std::string>();
//			break;
//		}
//		entt::hashed_string texturePath{ imageName.c_str() };
//
//		mapCache.load<MapLoader>(filename, Json, GetTexture(texturePath));
//		return mapCache.handle(filename).get();
//	}
//	const sf::Font& GetFont(entt::hashed_string filename)
//	{
//		if (fontCache.contains(filename))
//		{
//			return fontCache.handle(filename).get();
//		}
//
//		fontCache.load<FontLoader>(filename, filename);
//		return fontCache.handle(filename).get();
//	}
//private:
//	entt::cache<sf::Texture> textureCache;
//	entt::cache<sf::Font> fontCache;
//	entt::cache<AnimationResource> animationCache;
//	entt::cache<MapResource> mapCache;
//};
