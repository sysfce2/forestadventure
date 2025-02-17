/*
 *	Copyright (C) 2024 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <functional>

#include "Animation/AnimationTraits.h"

namespace FA {

namespace Shared {

template <class T>
class AnimationIf;

}  // namespace Shared

namespace Entity {

template <class FrameT>
class AnimatorIf
{
protected:
    using DrawableType = typename Shared::AnimationTraits<FrameT>::DrawableT;
    using UpdateCb = std::function<void(DrawableType &drawable, const Shared::AnimationIf<FrameT> &)>;

public:
    virtual ~AnimatorIf() = default;

    virtual void Enter() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void RegisterUpdateCb(UpdateCb updateCb) = 0;
};

}  // namespace Entity

}  // namespace FA
