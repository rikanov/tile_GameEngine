/*
 * Copyright (C) 2018  Robert Ikanov, robert.ikanov@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef ENUMS__H
#define ENUMS__H

enum class Ally
{
    FOE = -1,
    NONE,
    OWN
};

inline int ally_index(const Ally& A)
{
    return A == Ally::OWN;
}

enum class Piece
{
    NONE = 0,
    AURADIN    = 1,
    HUNTER     = 2,
    TELEPORTER = 4,
    WARRIOR    = AURADIN | HUNTER,
    GUARDIAN   = AURADIN | TELEPORTER,
    ASSASSIN   = HUNTER  | TELEPORTER,
    COMMANDER  = AURADIN | HUNTER | TELEPORTER
};

inline bool isMarchingGroup(const Piece& p1, const Piece& p2)
{
    return 3 & static_cast<int>(p1) & static_cast<int>(p2);
}
inline bool isMarchingGroup(const Piece& p1, const Piece& p2, const Piece& p3)
{
    return 3 & static_cast<int>(p1) & static_cast<int>(p2) & static_cast<int>(p3);
}
inline bool isDefender(const Piece& p)
{
    return static_cast<int>(p) & 1;
}
inline bool isAttacker(const Piece& p)
{
    return static_cast<int>(p) & 3;
}
inline bool isAuradin(const Piece& p)
{
    return static_cast<int>(p) & 1;
}
inline bool isHunter(const Piece& p)
{
    return static_cast<int>(p) & 2;
}
inline bool isTeleporter(const Piece& p)
{
    return static_cast<int>(p) & 4;
}
#endif
