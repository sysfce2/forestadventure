/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <map>
#include <tuple>
#include <vector>

#include "Animation/Animation.h"
#include "Enum/EntityType.h"
#include "Enum/FaceDirection.h"
#include "Enum/FrameType.h"

namespace FA {

class SheetManager;

struct AnimationData
{
    std::string sheetId_;
    struct LocationData
    {
        sf::Vector2u start_;
        unsigned int nFrames_;
        unsigned int defaultFrame_ = 0;
    };

    LocationData data_;
    bool mirror_;
    FrameType frameType_;
    FaceDirection dir_;
};

class AnimationDb
{
public:
    AnimationDb(SheetManager &sheetManager);
    void Load(EntityType entityType, const std::vector<AnimationData> &animationData);
    Animation GetAnimation(EntityType entityType, FrameType frameType, FaceDirection faceDir) const;

private:
    using Key = std::tuple<EntityType, FrameType, FaceDirection>;
    std::map<Key, Animation> map_;
    SheetManager &sheetManager_;

private:
    void AddAnimation(Key k, const Animation &animation);
};

}  // namespace FA
