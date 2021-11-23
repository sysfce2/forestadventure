/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <vector>

#include "Entity/EntityService.h"
#include "Entity/Id.h"
#include "Entity/ModeController.h"
#include "Fwd/SfmlFwd.h"

namespace FA {

namespace Keyboard {
enum class Key;
}

class MessageBus;
class Message;
enum class MessageType;
class CameraManager;

namespace Entity {

struct PropertyData;

class BasicEntity
{
public:
    BasicEntity(EntityId id, EntityType entityType, CameraManager& cameraManager, const AnimationDb& animationDb,
                MessageBus& messageBus);
    virtual ~BasicEntity();

    virtual std::string Name() const = 0;
    virtual void OnDestroy() {}

    void Create(const PropertyData& data);
    void Init();
    void Update(float deltaTime);
    void DrawTo(sf::RenderTarget& renderTarget);
    void EnableInput(bool enable) { enableInput_ = enable; }
    EntityId GetId() const { return id_; }

protected:
    virtual void HandleIsKeyPressed(Keyboard::Key key) {}
    virtual void HandleIsKeyReleased(Keyboard::Key key) {}
    virtual void HandleKeyPressed(Keyboard::Key key) {}

    void Subscribe(const std::vector<MessageType>& messageTypes);
    void Unsubscribe(const std::vector<MessageType>& messageTypes);
    void HandleEvent(std::shared_ptr<BasicEvent> event);

private:
    EntityId id_ = InvalidEntityId;
    MessageBus& messageBus_;
    EntityService entityService_;
    ModeController modeController_;
    bool enableInput_ = true;

private:
    virtual void DefineProperties(EntityService& entityService, const PropertyData& data) {}
    virtual void DefineModes(ModeController& modeController) {}
    virtual void DefineShape(EntityService& entityService, Shape& shape) {}

    void OnMessage(std::shared_ptr<Message> msg);
    void HandleMessage(std::shared_ptr<Message> msg);
};

}  // namespace Entity

}  // namespace FA
