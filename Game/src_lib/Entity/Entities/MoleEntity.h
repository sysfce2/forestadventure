/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

namespace FA {

namespace Entity {

class MoleEntity : public BasicEntity
{
public:
    MoleEntity(EntityId id, CameraManager& cameraManager, const SheetManager& sheetManager,
               EntityManager& entityManager, MessageBus& messageBus);
    virtual ~MoleEntity();

    virtual std::string Name() const override { return "MoleEntity"; }
    virtual EntityType Type() const override { return EntityType::Mole; }

protected:
    virtual std::vector<AnimationData> AnimationDataStore() const override;

private:
    virtual void AddAttributes(EntityService& handler, const AttributeData& data) override;
    virtual void RegisterModes(ModeController& modeController, const EntityService& entityService) override;
};

}  // namespace Entity

}  // namespace FA
