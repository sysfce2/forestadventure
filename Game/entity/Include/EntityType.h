/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <ostream>

namespace FA {

namespace Entity {

enum class EntityType { Unknown, Rect, Player, Mole, Arrow, Coin, Entrance };

inline std::ostream& operator<<(std::ostream& os, const EntityType& e)
{
    std::string str;
    switch (e) {
        case EntityType::Arrow:
            str = "Arrow";
            break;
        case EntityType::Coin:
            str = "Coin";
            break;
        case EntityType::Entrance:
            str = "Entrance";
            break;
        case EntityType::Mole:
            str = "Mole";
            break;
        case EntityType::Player:
            str = "Player";
            break;
        case EntityType::Rect:
            str = "Rect";
            break;
        case EntityType::Unknown:
            str = "Unknown";
            break;
    }

    os << str;

    return os;
}

}  // namespace Entity

}  // namespace FA
