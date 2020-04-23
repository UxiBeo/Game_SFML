#pragma once
#include "SFML/Graphics.hpp"
#include "entt/core/hashed_string.hpp"
#include "MaxxUtility.h"
//for load tile map

struct TextureResource
{
	struct TextureLoader final : entt::loader<TextureLoader, TextureResource> {
		std::shared_ptr<TextureResource> load(entt::hashed_string filename) const
		{
			auto resource = std::make_shared<TextureResource>();

			if (!resource->data.loadFromFile(static_cast<const char*>(filename)))
			{
				assert(false);
			}

			return resource;
		}
	};

	using Loader = TextureLoader;
	sf::Texture data;
};

struct FontResource
{
	struct FontLoader final : entt::loader<FontLoader, FontResource> {
		std::shared_ptr<FontResource> load(entt::hashed_string filename) const
		{
			auto resource = std::make_shared<FontResource>();

			if (!resource->data.loadFromFile(static_cast<const char*>(filename)))
			{
				assert(false);
			}

			return resource;
		}
	};
	using Loader = FontLoader;
	sf::Font data;
};



