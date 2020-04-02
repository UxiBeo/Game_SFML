#pragma once
#include "SFML/Graphics/Texture.hpp"
#include "entt/core/hashed_string.hpp"
#include "MaxxUtility.h"
#include "Codex.h"

//for load tile map
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
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
struct AnimationResource
{
	struct AnimationSet
	{
		entt::hashed_string nameSet;
		std::pair<unsigned char, unsigned char> rangeIndex;
	};

	struct AnimationLoader final : entt::loader<AnimationLoader, AnimationResource> {
		std::shared_ptr<AnimationResource> load(entt::hashed_string filename) const
		{
			const nlohmann::json Json = MaxxUtility::GetJson(filename);

			std::string imageName{ "Data\\Images\\" };
			imageName += Json["imageName"].get<std::string>();
			const entt::hashed_string textureName{ imageName.c_str() };
			Codex<TextureResource>::LoadFromFile(textureName);
			auto resource = std::make_shared<AnimationResource>();
			resource->textureName = textureName;
			resource->width = Json["width"].get<unsigned int>();
			resource->height = Json["height"].get<unsigned int>();
			resource->tileWidth = Json["tileWidth"].get<unsigned int>();
			resource->tileHeight = Json["tileHeight"].get<unsigned int>();
			resource->frameTime = Json["frameTime"].get<unsigned int>();
			for (auto& set : Json["animationSets"])
			{
				entt::hashed_string nameSet{ set["id"].get<std::string>().c_str() };
				resource->sets.push_back({ nameSet, set["index"].get<std::pair<unsigned char,unsigned char>>() });
			}
			return resource;
		}
	};
	const sf::IntRect GetRectByIndex(unsigned int Index) const
	{
		unsigned int nCot = width / tileWidth;
		unsigned int iHang = Index / nCot;
		unsigned int iCot = Index % nCot;
		return sf::IntRect(iCot * tileWidth, iHang * tileHeight, tileWidth, tileHeight);
	}
	const std::pair<unsigned char, unsigned char> FindByHashedString(entt::hashed_string name) const
	{
		for (size_t i = 0; i < sets.size(); i++)
		{
			if (sets[i].nameSet == name)
			{
				return sets[i].rangeIndex;
			}
		}
		return std::make_pair(0u, 0u);
	}
	using Loader = AnimationLoader;
	
	std::vector<AnimationSet> sets;
	entt::hashed_string textureName;
	unsigned int width, height, tileWidth, tileHeight, frameTime;
};

struct MapResource
{
	struct MapLoader final : entt::loader<MapLoader, MapResource> {
		std::shared_ptr<MapResource> load(entt::hashed_string filename) const
		{

			const nlohmann::json Json = MaxxUtility::GetJson(filename);

			std::string imageName{ "Data\\Images\\" };
			imageName += Json["imageName"].get<std::string>();
			entt::hashed_string texturePath{ imageName.c_str() };
			
			auto resource = std::make_shared<MapResource>();

			resource->textureName = texturePath;
			Codex<TextureResource>::LoadFromFile(texturePath);
			resource->tileSize = Json["tilewidth"].get<int>();
			resource->gridW = Json["width"].get<int>();
			resource->gridH = Json["height"].get<int>();

			//load layer map
			for (auto& layer : Json["layers"])
			{
				//add layer
				if (layer.find("data") != layer.end())
				{
					if (layer["name"].get<std::string>() == "TileObject")
					{
						resource->tilesWithObject = layer["data"].get<std::vector<unsigned int>>();
					}
					else
					{
						resource->layers.emplace_back(layer["data"].get<std::vector<unsigned int>>());
					}

					continue;
				}
				// add object layer
				if (layer.find("objects") != layer.end())
				{
					for (auto& object : layer["objects"])
					{
						resource->objectLayer.emplace_back(MapResource::Object{
							object["height"].get<float>(),
							object["width"].get<float>(),
							object["x"].get<float>(),
							object["y"].get<float>(),
							object["rotation"].get<unsigned int>()
							});
					}
				}
			}

			//load ofject per tile
			for (auto& tileset : Json["tilesets"])
			{
				for (auto& tile : tileset["tiles"])
				{
					if (tile.find("objectgroup") != tile.end())
					{
						const unsigned int id = tile["id"].get<unsigned int>() + 1u;
						for (auto& object : tile["objectgroup"]["objects"])
						{
							resource->objects.emplace(id, MapResource::Object{
								object["height"].get<float>(),
								object["width"].get<float>(),
								object["x"].get<float>(),
								object["y"].get<float>(),
								object["rotation"].get<unsigned int>()
								});
						}
					}
				}
			}


			return resource;
		}
	};
	using Loader = MapLoader;

	struct Object
	{
		float height, width, x, y;
		unsigned int rotation;
	};
	int tileSize = 0, gridW = 0, gridH = 0;
	entt::hashed_string textureName{};
	//layers tile
	std::vector<std::vector<unsigned int>> layers;

	//layer with tile has object
	std::vector<unsigned int> tilesWithObject;

	//object attached per tile
	std::unordered_map<unsigned int, Object> objects;

	//custom object, will add more in the future with sensor or static objects
	std::vector<Object> objectLayer;
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

struct TileMapResource
{
	struct TileMapLoader final : entt::loader<TileMapLoader, TileMapResource> {
		std::shared_ptr<TileMapResource> load(entt::hashed_string filename) const
		{
			auto resource = std::make_shared<TileMapResource>();

			if (!resource->map.load(static_cast<const char*>(filename)))
				assert(false && "can't load map");
			

			sf::Image fallback;
			fallback.create(2, 2, sf::Color::Magenta);
			for (const auto& ts : resource->map.getTilesets())
			{
				auto& texture = Codex<TextureResource>::RetrieveNonConst(entt::hashed_string{ ts.getImagePath().c_str() });
				sf::Image img;

				img.loadFromFile(ts.getImagePath());

				if (ts.hasTransparency())
				{
					auto transparency = ts.getTransparencyColour();
					img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
				}
				texture.data.loadFromImage(img);
			}
			return resource;
		}
	};
	using Loader = TileMapLoader;
	tmx::Map map;
};

