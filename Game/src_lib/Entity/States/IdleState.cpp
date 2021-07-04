/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "IdleState.h"

#include "AttackState.h"
#include "AttackWeaponState.h"
#include "Entity/Components/Sprite/Sprite.h"
#include "Enum/FrameType.h"
#include "MoveState.h"

namespace FA {

namespace Entity {

IdleState::IdleState(StateMachine& stateMachine, StateData& stateData)
    : BasicState(stateMachine, stateData)
{}

IdleState::~IdleState() = default;

void IdleState::Update(float deltaTime)
{
    stateData_.sprite_.Update(deltaTime);
}

void IdleState::DrawTo(sf::RenderTarget& renderTarget)
{
    stateData_.sprite_.DrawTo(renderTarget);
}

void IdleState::Enter()
{
    stateData_.sprite_.SetAnimation(FrameType::Idle, stateData_.faceDir_);
    stateData_.sprite_.StartAnimation();
}

void IdleState::OnStartMove(MoveDirection moveDir, FaceDirection faceDir)
{
    stateData_.moveDir_ = moveDir;
    stateData_.faceDir_ = faceDir;
    SwitchState<MoveState>();
}

void IdleState::OnAttack()
{
    SwitchState<AttackState>();
}

void IdleState::OnAttackWeapon()
{
    SwitchState<AttackWeaponState>();
}

}  // namespace Entity

}  // namespace FA
