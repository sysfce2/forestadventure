/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>
#include <unordered_map>

#include "Entity/Action.h"
#include "Entity/EventType.h"
#include "Entity/UpdateInfo.h"
#include "Enum/FrameType.h"
#include "Fwd/SfmlFwd.h"

namespace FA {

namespace Entity {

struct BasicEvent;
class Shape;
class EntityService;

class BasicMode
{
public:
    BasicMode(EntityService* entityService);
    virtual ~BasicMode();

    BasicMode(const BasicMode&) = delete;
    BasicMode& operator=(const BasicMode&) = delete;
    BasicMode(BasicMode&&) = delete;
    BasicMode& operator=(BasicMode&&) = delete;

    virtual void Enter(std::shared_ptr<BasicEvent> event) {}
    virtual void Exit() {}
    virtual void Update(float deltaTime) {}
    virtual void DrawTo(sf::RenderTarget& renderTarget);
    virtual void Awake() {}

    virtual FrameType GetFrameType() const = 0;
    virtual ModeType GetModeType() const = 0;

    void AddEvent(EventType eventType, ModeType modeType, std::function<void(std::shared_ptr<BasicEvent> event)> cb);
    void AddUpdateFn(std::function<bool(std::shared_ptr<Shape>)> updateFn, ModeType modeType);
    Action GetAction(EventType eventType) const;
    UpdateInfo GetUpdateInfo() const;

    ModeType PollUpdate() const;

protected:
    EntityService* Service() const { return entityService_; }

private:
    EntityService* entityService_ = nullptr;
    std::unordered_map<EventType, Action> eventMap_;
    UpdateInfo updateInfo_{};
};

}  // namespace Entity

}  // namespace FA
