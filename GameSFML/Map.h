#pragma once
#include "entt/entt.hpp"
#include "MaxxUtility.h"
#include "SFML/Graphics.hpp"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "GameResource.h"
#include "Codex.h"
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
class Map : public sf::Drawable
{
public:
    void Load(const std::string& filename);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void ClipView();

private:
    void LoadTileLayer(tmx::Layer& layer, const tmx::Tileset& tileset);
    void LoadObjectlayer(tmx::Layer& layer);
    void LoadLayerImage(tmx::Layer& layer);
private:
    sf::Vector2u mapSize;
    sf::VertexArray vertexArray;
    std::vector<std::pair<uint32_t, uint32_t>> chunks;
    std::unique_ptr<tmx::Map> map;
    sf::Texture texture;
    uint32_t firstId; //First tile id of the tileset
    uint32_t columns; //For the demo map it is 8.
    uint32_t rows;
    uint32_t lastId;
};