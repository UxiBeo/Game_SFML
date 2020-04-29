#pragma once
#include "entt/entt.hpp"
#include "MaxxUtility.h"
#include "SFML/Graphics.hpp"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <optional>
class Map : public sf::Drawable
{
public:
    void Load(const std::string& filename);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void ClipView();
    void PrepareDraw();
private:
    void LoadTileLayer(tmx::Layer& layer, const tmx::Tileset& tileset);
    void LoadObjectlayer(tmx::Layer& layer);
    void LoadLayerImage(tmx::Layer& layer);
private:
    sf::Vector2u mapSize;
    sf::VertexArray vertexArray;
    std::vector<std::pair<uint32_t, uint32_t>> chunks;
    std::unique_ptr<tmx::Map> map;
    const sf::Texture* texture = nullptr;
    const std::vector<tmx::TileLayer::Tile>* bgTiles = nullptr;
    uint32_t firstId; //First tile id of the tileset
    uint32_t columns; //For the demo map it is 8.
    uint32_t rows;
    uint32_t lastId;
};