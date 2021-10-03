/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "PropertyHandler.h"

#include "Attributes//CameraAttribute.h"
#include "Attributes//FaceDirectionAttribute.h"
#include "Attributes//TransformAttribute.h"
#include "Attributes/AnimationAttribute.h"
#include "Behaviors/AttackBehavior.h"
#include "Behaviors/AttackWeaponBehavior.h"
#include "Behaviors/IdleBehavior.h"
#include "Behaviors/MovementBehavior.h"
#include "Level/CameraManager.h"
#include "Resource/AnimationDb.h"

namespace FA {

namespace Entity {

PropertyHandler::PropertyHandler(const PropertyData& data, CameraManager& cameraManager)
    : data_(data)
    , cameraManager_(cameraManager)
{
    auto t = attributeStore_.AddProperty<TransformAttribute>(this);
    t->SetPosition(data.position_);
    t->SetScale(data.scale_);
    behaviorStore_.AddProperty<IdleBehavior>(this);
    frameTypes_.push_back(FrameType::Idle);
}

PropertyHandler::~PropertyHandler() = default;

template <>
std::shared_ptr<MovementBehavior> PropertyHandler::AddBehavior<MovementBehavior>()
{
    frameTypes_.push_back(FrameType::Move);
    auto m = behaviorStore_.AddProperty<MovementBehavior>(this);
    m->SetVelocity(data_.velocity_);
    return m;
}

template <>
std::shared_ptr<AttackBehavior> PropertyHandler::AddBehavior<AttackBehavior>()
{
    frameTypes_.push_back(FrameType::Attack);
    return behaviorStore_.AddProperty<AttackBehavior>(this);
}

template <>
std::shared_ptr<AttackWeaponBehavior> PropertyHandler::AddBehavior<AttackWeaponBehavior>()
{
    frameTypes_.push_back(FrameType::AttackWeapon);
    return behaviorStore_.AddProperty<AttackWeaponBehavior>(this);
}

template <>
std::shared_ptr<CameraAttribute> PropertyHandler::AddAttribute<CameraAttribute>()
{
    auto t = GetAttribute<TransformAttribute>();
    cameraManager_.Track(t->GetPosition());
    return attributeStore_.AddProperty<CameraAttribute>(this);
}

void PropertyHandler::Awake()
{
    attributeStore_.Awake();
    behaviorStore_.Awake();
}

void PropertyHandler::InitProperties(const AnimationDb& animationDb)
{
    auto dirs = attributeStore_.GetProperty<FaceDirectionAttribute>()->GetAvailableDirections();

    for (auto f : frameTypes_) {
        for (auto face : dirs) {
            auto animation = animationDb.GetAnimation(data_.entityType_, f, face);
            attributeStore_.GetProperty<AnimationAttribute>()->AddAnimation(f, face, animation);
        }
    }
}

}  // namespace Entity

}  // namespace FA
