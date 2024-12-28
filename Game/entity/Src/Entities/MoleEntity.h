/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <unordered_map>

#include "BasicEntity.h"

#include "Animation/ImageAnimation.h"
#include "Enum/FaceDirection.h"
#include "Enum/MoveDirection.h"
#include "Resource/ColliderData.h"
#include "Resource/ImageData.h"
#include "ShapeParts/MultiAnimationPart.h"

namespace FA {

namespace Entity {

class MoleEntity : public BasicEntity
{
public:
    static const std::string str;

    MoleEntity(EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> service);
    virtual ~MoleEntity();

    virtual EntityType Type() const override { return EntityType::Mole; }
    virtual LayerType GetLayer() const override { return LayerType::Ground; }
    virtual bool IsStatic() const override { return false; }
    virtual bool IsSolid() const override { return false; }

private:
    virtual void RegisterProperties() override;
    virtual void ReadProperties(const std::unordered_map<std::string, std::string>& properties) override;
    virtual void RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                const Shared::EntityData& data) override;

    void OnBeginMove(MoveDirection moveDirection);
    void OnUpdateMove(const sf::Vector2f& delta);

    std::shared_ptr<MultiAnimationPart<FaceDirection, Shared::ImageFrame>> MakeShapePart(
        const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>>& faceDirImages,
        Graphic::SpriteIf& sprite);
    std::shared_ptr<MultiAnimationPart<FaceDirection, Shared::ColliderFrame>> MakeColliderPart(
        const std::unordered_map<FaceDirection, std::vector<Shared::ColliderData>>& faceDirColliders,
        Graphic::RectangleShapeIf& rect);

    void DefineIdleState(std::shared_ptr<State> state);
    void DefineMoveState(std::shared_ptr<State> state);
    void DefineCollisionState(std::shared_ptr<State> state);
};

}  // namespace Entity

}  // namespace FA
