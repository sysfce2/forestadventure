/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Enum/EntityType.h"
#include "Id.h"
#include "Resource/TextureManager.h"

namespace FA {

namespace Shared {

class CameraViews;
class CameraView;
class ImageAnimation;
class Collider;
class SheetManager;
struct ImageData;
struct ColliderData;
class MessageBus;
class Message;
enum class MessageType;
struct MapData;
struct TextureRect;

}  // namespace Shared

namespace Entity {

struct PropertyData;
class EntityManager;
enum class EntityType;

class EntityService
{
public:
    EntityService(Shared::MessageBus &messageBus, const Shared::TextureManager &textureManager,
                  const Shared::SheetManager &sheetManager, const Shared::CameraViews &cameraViews,
                  EntityManager &entityManager);
    ~EntityService();

    Shared::ImageAnimation MakeAnimation(const std::vector<Shared::ImageData> &data) const;
    Shared::Collider MakeCollider(const std::vector<Shared::ColliderData> &data) const;

    void SendMessage(std::shared_ptr<Shared::Message> msg);
    void AddSubscriber(const std::string &subscriber, const std::vector<Shared::MessageType> &messageTypes,
                       std::function<void(std::shared_ptr<Shared::Message>)> onMessage);

    void RemoveSubscriber(const std::string &subscriber, const std::vector<Shared::MessageType> &messageTypes);
    Shared::CameraView &GetCameraView() const;
    void CreateEntity(const PropertyData &data, const Shared::MapData &mapData);
    void DeleteEntity(EntityId id);
    EntityType GetType(EntityId id) const;

private:
    Shared::MessageBus &messageBus_;
    const Shared::TextureManager &textureManager_;
    const Shared::SheetManager &sheetManager_;
    const Shared::CameraViews &cameraViews_;
    EntityManager &entityManager_;

private:
    Shared::TextureRect MirrorX(const Shared::TextureRect &textureRect) const;
};

}  // namespace Entity

}  // namespace FA
