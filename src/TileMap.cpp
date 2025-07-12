#include "../include/TileMap.h"
#include "../tinyxml2.h"
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>

using namespace tinyxml2;

bool TileMap::loadFromTMX(const std::string& filename, const sf::Texture& tileset, int tw, int th) {
    tileWidth = tw;
    tileHeight = th;
    tilesetTexture = &tileset;

    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading TMX file\n";
        return false;
    }

    XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) return false;

    widthInTiles = mapElement->IntAttribute("width");
    heightInTiles = mapElement->IntAttribute("height");

    XMLElement* layer = mapElement->FirstChildElement("layer");
    XMLElement* data = layer ? layer->FirstChildElement("data") : nullptr;
    if (!data || !data->GetText()) return false;

    std::string csvStr = data->GetText();
    csvStr.erase(std::remove_if(csvStr.begin(), csvStr.end(), [](unsigned char c) {
        return c == '\n' || c == '\r' || c == ' ';
        }), csvStr.end());

    std::istringstream ss(csvStr);
    std::string gidStr;

    mapData.clear();
    tileTypes.clear();
    mapData.resize(heightInTiles, std::vector<int>(widthInTiles, 0));
    tileTypes.resize(heightInTiles, std::vector<TileType>(widthInTiles, TileType::empty));

    int y = 0, x = 0;
    while (std::getline(ss, gidStr, ',')) {
        if (gidStr.empty()) continue;
        int gid = std::stoi(gidStr);
        mapData[y][x] = gid;
        x++;
        if (x >= widthInTiles) { x = 0; y++; }
        if (y >= heightInTiles) break;
    }

    // 解析 tileset，支持外部 tsx 文件
    XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (!tilesetElement) {
        std::cerr << "No tileset element found\n";
        return false;
    }

    int firstGid = tilesetElement->IntAttribute("firstgid");

    std::map<int, TileType> gidToType;

    const char* source = tilesetElement->Attribute("source");
    if (source) {
        // 处理外部 tsx 文件
        std::string tmxDir;
        auto pos = filename.find_last_of("/\\");
        if (pos != std::string::npos)
            tmxDir = filename.substr(0, pos + 1);
        std::string tsxPath = tmxDir + source;

        XMLDocument tsxDoc;
        if (tsxDoc.LoadFile(tsxPath.c_str()) != XML_SUCCESS) {
            std::cerr << "Failed to load tsx file: " << tsxPath << std::endl;
            return false;
        }

        XMLElement* tsxTileset = tsxDoc.FirstChildElement("tileset");
        if (!tsxTileset) {
            std::cerr << "No tileset element in tsx file\n";
            return false;
        }

        // 解析 tsx 文件内的 tile properties
        for (XMLElement* tile = tsxTileset->FirstChildElement("tile"); tile; tile = tile->NextSiblingElement("tile")) {
            int id = tile->IntAttribute("id");
            XMLElement* properties = tile->FirstChildElement("properties");
            if (properties) {
                for (XMLElement* prop = properties->FirstChildElement("property"); prop; prop = prop->NextSiblingElement("property")) {
                    const char* nameAttr = prop->Attribute("name");
                    const char* valueAttr = prop->Attribute("value");

                    if (nameAttr && valueAttr && std::string(nameAttr) == "type") {
                        TileType type = TileType::empty;
                        std::string value = valueAttr;
                        if (value == "ground") type = TileType::ground;
                        else if (value == "water") type = TileType::water;
                        else if (value == "ladder" || value == "stair") type = TileType::ladder;
                        else if (value == "obstacle") type = TileType::obstacle;
                        else if (value == "exit") type = TileType::exit;

                        gidToType[firstGid + id] = type;
                    }
                }
            }
        }

    }
    else {
        // 处理内嵌 tileset 的情况
        for (XMLElement* tile = tilesetElement->FirstChildElement("tile"); tile; tile = tile->NextSiblingElement("tile")) {
            int id = tile->IntAttribute("id");
            XMLElement* properties = tile->FirstChildElement("properties");
            if (properties) {
                for (XMLElement* prop = properties->FirstChildElement("property"); prop; prop = prop->NextSiblingElement("property")) {
                    const char* nameAttr = prop->Attribute("name");
                    const char* valueAttr = prop->Attribute("value");

                    if (nameAttr && valueAttr && std::string(nameAttr) == "type") {
                        TileType type = TileType::empty;
                        std::string value = valueAttr;
                        if (value == "ground") type = TileType::ground;
                        else if (value == "water") type = TileType::water;
                        else if (value == "ladder" || value == "stair") type = TileType::ladder;
                        else if (value == "obstacle") type = TileType::obstacle;
                        else if (value == "exit") type = TileType::exit;

                        gidToType[firstGid + id] = type;
                    }
                }
            }
        }
    }

    // 赋值 tileTypes，根据 gid 查找对应类型，没有则默认 empty
    for (int i = 0; i < heightInTiles; ++i) {
        for (int j = 0; j < widthInTiles; ++j) {
            int gid = mapData[i][j];
            tileTypes[i][j] = gidToType.count(gid) ? gidToType[gid] : TileType::empty;
        }
    }

    // 构建顶点数组，准备渲染（不变）
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(widthInTiles * heightInTiles * 6);

    int tilesPerRow = tileset.getSize().x / tileWidth;

    for (int i = 0; i < heightInTiles; ++i) {
        for (int j = 0; j < widthInTiles; ++j) {
            int gid = mapData[i][j];
            if (gid == 0) continue;

            int localId = gid - firstGid;
            if (localId < 0) continue;

            int tx = localId % tilesPerRow;
            int ty = localId / tilesPerRow;
            int index = (i * widthInTiles + j) * 6;

            sf::Vector2f pos(j * tileWidth, i * tileHeight);
            sf::Vector2f size(tileWidth, tileHeight);
            sf::Vector2f tex(tx * tileWidth, ty * tileHeight);

            vertices[index + 0].position = pos;
            vertices[index + 1].position = pos + sf::Vector2f(size.x, 0);
            vertices[index + 2].position = pos + size;

            vertices[index + 3].position = pos + size;
            vertices[index + 4].position = pos + sf::Vector2f(0, size.y);
            vertices[index + 5].position = pos;

            vertices[index + 0].texCoords = tex;
            vertices[index + 1].texCoords = tex + sf::Vector2f(size.x, 0);
            vertices[index + 2].texCoords = tex + size;

            vertices[index + 3].texCoords = tex + size;
            vertices[index + 4].texCoords = tex + sf::Vector2f(0, size.y);
            vertices[index + 5].texCoords = tex;
        }
    }    
    return true;
}


int TileMap::getTileWidth() const {
    return tileWidth;
}

int TileMap::getTileHeight() const {
    return tileHeight;
}

TileType TileMap::getTileType(int row, int col) const {
    if (row < 0 || row >= (int)tileTypes.size() || col < 0 || col >= (int)tileTypes[0].size())
        return TileType::empty;
    return tileTypes[row][col];
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tilesetTexture;
    target.draw(vertices, states);
}

TileType TileMap::getTileTypeAt(float x, float y) const {
    int col = static_cast<int>(x) / tileWidth;
    int row = static_cast<int>(y) / tileHeight;

    if (row >= 0 && row < heightInTiles && col >= 0 && col < widthInTiles) {
        return tileTypes[row][col];
    }
    else {
        return TileType::empty; // 地图外视为空地
    }
}