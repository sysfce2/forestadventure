/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "LevelComponent.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Effect/BasicEffect.h"
#include "Folder.h"
#include "Logging.h"
#include "Message/MessageBus.h"
#include "Misc/TextureManager.h"
#include "Scene/Layer.h"
#include "Scene/Transitions/BasicTransition.h"

namespace FA {

namespace Scene {

LevelComponent::LevelComponent(MessageBus& messageBus, const Layer& layer, TextureManager& textureManager)
    : BasicComponent(messageBus, layer)
    , camera_(layerTexture_.getSize())
{
    LOG_INFO_ENTER_FUNC();
    auto path = GetAssetsPath() + "/map/test.tmx";
    auto tileMapData = tileMapReader_.Parse(path);
    level_ = std::make_unique<Level>(tileMapData, textureManager);
    level_->Create(camera_, messageBus);
    LOG_INFO_EXIT_FUNC();
}

LevelComponent::~LevelComponent() = default;

void LevelComponent::Draw()
{
    level_->DrawTo(layerTexture_);
    if (effect_) effect_->DrawTo(layerTexture_);
}

void LevelComponent::Update(float deltaTime)
{
    level_->Update(deltaTime);
    if (effect_) effect_->Update(deltaTime);
    camera_.UpdatePosition(layerTexture_, level_->GetSize());
}

void LevelComponent::EnableInput(bool enable)
{
    level_->EnableInput(enable);
}

void LevelComponent::EnterTransition(const BasicTransition& transition)
{
    sf::Vector2f layerPos = layerTexture_.mapPixelToCoords({0, 0});
    sf::Vector2f layerSize = static_cast<sf::Vector2f>(layerTexture_.getSize());
    effect_ = transition.CreateEffect(layerPos, layerSize);
}

void LevelComponent::ExitTransition(const BasicTransition& transition)
{
    effect_.reset();
}

}  // namespace Scene

}  // namespace FA
