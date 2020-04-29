#include "Map.h"
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

void Map::Load(const std::string& filename)
{
    map = std::make_unique<tmx::Map>();
    map->load(filename);
    auto& tileset = *(map->getTilesets().begin());
    texture = &Codex<TextureResource>::Retrieve(entt::hashed_string{ tileset.getImagePath().c_str() }).data;
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
    float scalePixel = 2.0f;
    firstId = tileset.getFirstGID(); //First tile id of the tileset
    columns = tileset.getColumnCount(); //For the demo map it is 8.
    rows = tileset.getTileCount() / columns;
    lastId = tileset.getLastGID();
    bgTiles = &layer.getLayerAs<tmx::TileLayer>().getTiles();
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(bgTiles->size() * 4);
    mapSize.x = map->getTileCount().x;
    mapSize.y = map->getTileCount().y;
    //pos = position in tile units
    sf::Vector2f tileSize = sf::Vector2f(map->getTileSize().x, map->getTileSize().y);;
    for (uint32_t i = 0; i < bgTiles->size(); i++)
    {
        sf::Vertex* quad = &vertexArray[i * 4u];
        if ((*bgTiles)[i].ID >= firstId && (*bgTiles)[i].ID <= lastId)
        {
            //With this, I know that it's related to the tileset above (though I only have one tileset)
            //Get position in pixel units
            sf::Vector2f position = { (i % mapSize.x) * tileSize.x * scalePixel, (i / mapSize.x) * tileSize.y * scalePixel };

            uint32_t baseTilePosition = ((*bgTiles)[i].ID - firstId);

            uint32_t tileModX = baseTilePosition % columns;
            uint32_t currentRow = baseTilePosition / columns;
            uint32_t offsetX = tileModX != 0 ? tileModX * tileSize.x : 0;
            uint32_t offsetY = currentRow < rows - 1 ? currentRow * tileSize.y : (rows - 1) * tileSize.y;

            // define its 4 corners
            quad[0].position = position;
            quad[1].position = sf::Vector2f(position.x + tileSize.x * scalePixel, position.y);
            quad[2].position = sf::Vector2f(position.x + tileSize.x * scalePixel, position.y + tileSize.y * scalePixel);
            quad[3].position = sf::Vector2f(position.x, position.y + tileSize.y * scalePixel);

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
    states.texture = texture;

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

void Map::PrepareDraw()
{
    float scalePixel = 2.0f;
    firstId; //First tile id of the tileset
    columns; //For the demo map it is 8.
    rows;
    lastId;
    bgTiles;

    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(bgTiles->size() * 4);
    mapSize.x = map->getTileCount().x;
    mapSize.y = map->getTileCount().y;
    //pos = position in tile units
    sf::Vector2f tileSize = sf::Vector2f(map->getTileSize().x, map->getTileSize().y);;
    for (uint32_t i = 0; i < bgTiles->size(); i++)
    {
        sf::Vertex* quad = &vertexArray[i * 4u];
        if ((*bgTiles)[i].ID >= firstId && (*bgTiles)[i].ID <= lastId)
        {

            //With this, I know that it's related to the tileset above (though I only have one tileset)
            //Get position in pixel units
            sf::Vector2f position = { (i % mapSize.x) * tileSize.x * scalePixel, (i / mapSize.x) * tileSize.y * scalePixel };

            uint32_t baseTilePosition = ((*bgTiles)[i].ID - firstId);

            uint32_t tileModX = baseTilePosition % columns;
            uint32_t currentRow = baseTilePosition / columns;
            uint32_t offsetX = tileModX != 0 ? tileModX * tileSize.x : 0;
            uint32_t offsetY = currentRow < rows - 1 ? currentRow * tileSize.y : (rows - 1) * tileSize.y;

            // define its 4 corners
            quad[0].position = position;
            quad[1].position = sf::Vector2f(position.x + tileSize.x * scalePixel, position.y);
            quad[2].position = sf::Vector2f(position.x + tileSize.x * scalePixel, position.y + tileSize.y * scalePixel);
            quad[3].position = sf::Vector2f(position.x, position.y + tileSize.y * scalePixel);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(offsetX, offsetY);
            quad[1].texCoords = sf::Vector2f(offsetX + tileSize.x, offsetY);
            quad[2].texCoords = sf::Vector2f(offsetX + tileSize.x, offsetY + tileSize.y);
            quad[3].texCoords = sf::Vector2f(offsetX, offsetY + tileSize.y);
        }
    }
}
