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

#include <SFML/Graphics/Rect.hpp>

#include "Enum/EntityType.h"
#include "Id.h"

namespace FA {

namespace Shared {

class CameraManager;
class Camera;
class Animation;
class Image;
class SheetManager;
struct AnimationData;
struct ImageData;
class MessageBus;
class Message;
enum class MessageType;
struct MapData;

}  // namespace Shared

namespace Entity {

struct PropertyData;
class EntityManager;

class EntityService
{
public:
    EntityService(Shared::MessageBus &messageBus, const Shared::SheetManager &sheetManager,
                  const Shared::CameraManager &cameraManager, EntityManager &entityManager);
    ~EntityService();

    Shared::Animation MakeAnimation(const Shared::AnimationData &data) const;
    Shared::Image MakeImage(const Shared::ImageData &data) const;

    void SendMessage(std::shared_ptr<Shared::Message> msg);
    void AddSubscriber(const std::string &subscriber, const std::vector<Shared::MessageType> &messageTypes,
                       std::function<void(std::shared_ptr<Shared::Message>)> onMessage);

    void RemoveSubscriber(const std::string &subscriber, const std::vector<Shared::MessageType> &messageTypes);
    Shared::Camera &GetCamera() const;
    void CreateEntity(const PropertyData &data, const Shared::MapData &mapData);
    void DeleteEntity(EntityId id);

private:
    Shared::MessageBus &messageBus_;
    const Shared::SheetManager &sheetManager_;
    const Shared::CameraManager &cameraManager_;
    EntityManager &entityManager_;
};

}  // namespace Entity

}  // namespace FA
