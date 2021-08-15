/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "AttackState.h"

#include "Entity/Components/Sprite/SpriteComponent.h"
#include "Enum/FrameType.h"
#include "IdleState.h"
#include "MoveState.h"

namespace FA {

namespace Entity {

AttackState::AttackState(StateMachine& stateMachine, StateData& stateData)
    : BasicState(stateMachine, stateData)
{}

AttackState::~AttackState() = default;

void AttackState::Update(float deltaTime)
{
    stateData_.configuration_->sprite_->Update(deltaTime);
    if (stateData_.configuration_->sprite_->AnimationIsCompleted()) {
        SwitchState<IdleState>();
    }
}

void AttackState::DrawTo(sf::RenderTarget& renderTarget)
{
    stateData_.configuration_->sprite_->DrawTo(renderTarget);
}

void AttackState::Enter()
{
    stateData_.configuration_->sprite_->Set(FrameType::Attack, stateData_.configuration_->faceDir_);
}

void AttackState::OnStartMove(MoveDirection moveDir, FaceDirection faceDir)
{
    stateData_.configuration_->movement_->SetDirection(moveDir);
    stateData_.configuration_->faceDir_ = faceDir;
    SwitchState<MoveState>();
}

}  // namespace Entity

}  // namespace FA
