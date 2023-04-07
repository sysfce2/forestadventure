/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

#include "Enum/MoveDirection.h"

namespace FA {

namespace Entity {

class PlayerEntity : public BasicEntity
{
public:
    static const std::string str;

    PlayerEntity(EntityId id, const PropertyData& data, const EntityService& service);
    virtual ~PlayerEntity();

    virtual EntityType Type() const override { return EntityType::Player; }
    virtual LayerType GetLayer() const override { return LayerType::Ground; }

protected:
    virtual std::vector<Shared::MessageType> Messages() const override;

private:
    virtual void RegisterProperties() override;
    virtual void ReadProperties(const std::unordered_map<std::string, std::string>& properties) override;
    virtual void RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                const PropertyData& data) override;
    virtual void OnInit() override;
    virtual void OnMessage(std::shared_ptr<Shared::Message> msg) override;
    virtual void OnBeginDie() override;

    void OnBeginMove(MoveDirection moveDirection);
    void OnUpdateMove(const sf::Vector2f& delta);
    void OnShoot();

    void RegisterIdleState(std::shared_ptr<State> state);
    void RegisterMoveState(std::shared_ptr<State> state);
    void RegisterAttackState(std::shared_ptr<State> state);
    void RegisterAttackWeaponState(std::shared_ptr<State> state);
};

}  // namespace Entity

}  // namespace FA
