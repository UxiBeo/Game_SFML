#include "Map.h"

void Map::Load(const std::string& filename)
{
    map = std::make_unique<tmx::Map>();
    map->load(filename);
    auto& tileset = *(map->getTilesets().begin());
    texture.loadFromFile(tileset.getImagePath());
    for (const auto& layer : map->getLayers())
    {
        switch (layer->getType())
        {
        case tmx::Layer::Type::Tile:
            LoadTileLayer(*layer, tileset);
            break;

        case tmx::Layer::Type::Object:
            LoadObjectlayer(*layer);
            break;

        case tmx::Layer::Type::Image:
            LoadLayerImage(*layer);
            break;

        default:
            break;
        }
    }
    //map.release();
}
void Map::LoadTileLayer(tmx::Layer& layer, const tmx::Tileset& tileset)
{
    firstId = tileset.getFirstGID(); //First tile id of the tileset
    columns = tileset.getColumnCount(); //For the demo map it is 8.
    rows = tileset.getTileCount() / columns;
    lastId = tileset.getLastGID();
    const auto& tiles = layer.getLayerAs<tmx::TileLayer>().getTiles();
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(tiles.size() * 4);
    mapSize.x = map->getTileCount().x;
    mapSize.y = map->getTileCount().y;
    //pos = position in tile units
    sf::Vector2f tileSize = sf::Vector2f(map->getTileSize().x, map->getTileSize().y);;
    for (uint32_t i = 0; i < tiles.size(); i++)
    {
        sf::Vertex* quad = &vertexArray[i * 4u];
        if (tiles[i].ID >= firstId && tiles[i].ID <= lastId)
        {

            //With this, I know that it's related to the tileset above (though I only have one tileset)
            //Get position in pixel units
            sf::Vector2f position = { (i % mapSize.x) * tileSize.x, (i / mapSize.x) * tileSize.y };

            uint32_t baseTilePosition = (tiles[i].ID - firstId);

            uint32_t tileModX = baseTilePosition % columns;
            uint32_t currentRow = baseTilePosition / columns;
            uint32_t offsetX = tileModX != 0 ? tileModX * tileSize.x : 0;
            uint32_t offsetY = currentRow < rows - 1 ? currentRow * tileSize.y : (rows - 1) * tileSize.y;

            // define its 4 corners
            quad[0].position = position;
            quad[1].position = sf::Vector2f(position.x + tileSize.x, position.y);
            quad[2].position = sf::Vector2f(position.x + tileSize.x, position.y + tileSize.y);
            quad[3].position = sf::Vector2f(position.x, position.y + tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(offsetX, offsetY);
            quad[1].texCoords = sf::Vector2f(offsetX + tileSize.x, offsetY);
            quad[2].texCoords = sf::Vector2f(offsetX + tileSize.x, offsetY + tileSize.y);
            quad[3].texCoords = sf::Vector2f(offsetX, offsetY + tileSize.y);
        }
    }
}
void Map::LoadObjectlayer(tmx::Layer& layer)
{

}
void Map::LoadLayerImage(tmx::Layer& layer)
{

}
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the texture
    states.texture = &texture;

    // you may also override states.shader or states.blendMode if you want

    // draw the vertex array
    target.draw(vertexArray, states);
}

void Map::ClipView()
{
    uint32_t top = 0;
    uint32_t bottom = 40;
    uint32_t left = 0;
    uint32_t right = 40;
    for (uint32_t y = top; y < bottom; y++)
    {
        chunks.emplace_back(y * mapSize.x + left, y * mapSize.x + right);
    }
}