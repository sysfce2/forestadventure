/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "TileMap.h"

#include "Logging.h"
#include "Resource/ImageData.h"
#include "Resource/ResourceId.h"
#include "Resource/SheetManager.h"
#include "Resource/SpriteSheet.h"
#include "Resource/TextureManager.h"
#include "TileMapData.h"
#include "TileMapParser.h"

namespace FA {

namespace World {

TileMap::TileMap(Shared::TextureManager& textureManager, Shared::SheetManager& sheetManager)
    : textureManager_(textureManager)
    , sheetManager_(sheetManager)
{
    tileMapParser_ = std::make_unique<Tile::TileMapParser>();
}

TileMap::~TileMap() = default;

void TileMap::Load(const std::string& fileName)
{
    tileMapData_ = std::make_unique<Tile::TileMapData>(tileMapParser_->Run(fileName));
    LoadTileSets();
}

void TileMap::Setup()
{
    LOG_INFO("Setup tile map");
    SetupLayers();
    SetupEntityGroups();
}

void TileMap::LoadTileSets()
{
    for (auto& entry : tileMapData_->tileSets_) {
        auto images = entry.second.images_;
        for (const auto& image : images) {
            Shared::ResourceId id = textureManager_.Load(image.path_);
            auto size = textureManager_.Get(id)->getSize();
            auto sheet = std::make_unique<Shared::SpriteSheet>(id, size, sf::Vector2u(image.nCols_, image.nRows_));
            sheetManager_.AddSheet(image.path_, std::move(sheet));
        }
    }
}

void TileMap::SetupLayers()
{
    auto nCols = tileMapData_->mapProperties_.width_;
    auto tileWidth = tileMapData_->mapProperties_.tileWidth_;
    auto tileHeight = tileMapData_->mapProperties_.tileHeight_;

    for (const auto& layer : tileMapData_->layers_) {
        int inx = 0;
        auto layerName = layer.name_;
        for (auto it = layer.tileIds_.begin(); layer.tileIds_.end() != it; ++it, ++inx) {
            auto tileId = *it;
            if (tileId == 0) continue;
            auto tileData = LookupTileData(tileId);
            TileMap::TileData outData;
            unsigned int x = (inx % nCols) * tileWidth;
            unsigned int y = (inx / nCols) * tileHeight;

            if (!tileData.animation_.empty()) {
                auto first = tileData.animation_.at(0);
                if (first.height_ > tileHeight) {
                    y += tileHeight;
                    y -= first.height_;
                }
            }

            outData.position_ = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

            if (!tileData.animation_.empty()) {
                std::vector<Shared::ImageData> outAnimation;
                for (auto frame : tileData.animation_) {
                    Shared::ImageData data{{frame.texturePath_, {frame.column_, frame.row_}}};
                    outAnimation.push_back(data);
                }

                outData.graphic_.animation_ = outAnimation;
            }

            Shared::ImageData data{{tileData.image_.texturePath_, {tileData.image_.column_, tileData.image_.row_}}};
            outData.graphic_.image_ = data;

            layers_[layerName].push_back(outData);
        }
    }
}

void TileMap::SetupEntityGroups()
{
    for (const auto& group : tileMapData_->objectGroups_) {
        auto groupName = group.name_;
        std::vector<Shared::EntityData> entityDatas;
        for (const auto& object : group.objects_) {
            Shared::EntityData entityData;
            entityData.objId_ = object.id_;
            entityData.position_ = sf::Vector2f(static_cast<float>(object.x_), static_cast<float>(object.y_));
            entityData.size_ = sf::Vector2f(static_cast<float>(object.width_), static_cast<float>(object.height_));
            entityData.typeStr_ = object.typeStr_;
            entityData.properties_ = object.properties_;
            entityDatas.push_back(entityData);
        }
        entityGroups_[groupName] = entityDatas;
    }
}

const std::vector<TileMap::TileData> TileMap::GetLayer(const std::string& name) const
{
    return layers_.at(name);
}

const std::vector<Shared::EntityData> TileMap::GetEntityGroup(const std::string& name) const
{
    return entityGroups_.at(name);
}

sf::Vector2u TileMap::GetSize() const
{
    auto nCols = tileMapData_->mapProperties_.width_;
    auto nRows = tileMapData_->mapProperties_.height_;
    auto tileWidth = tileMapData_->mapProperties_.tileWidth_;
    auto tileHeight = tileMapData_->mapProperties_.tileHeight_;
    auto size = sf::Vector2u(nCols * tileWidth, nRows * tileHeight);

    return size;
}

Tile::TileData TileMap::LookupTileData(int id)
{
    auto it = tileMapData_->tileSets_.lower_bound(id);

    if (it != tileMapData_->tileSets_.end()) {
        auto firstGid = it->first;
        return it->second.lookupTable_[id - firstGid];
    }
    else {
        LOG_ERROR("%s not found", DUMP(id));
        return {};
    }
}

}  // namespace World

}  // namespace FA
