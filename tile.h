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

#ifndef TILE_H
#define TILE_H

#include "enums.h"

class Field;
class Tile
{
    static double value_norm;
    static const double StandardValues[];
    static const char* TileNamesToText[];
    
    Ally opponent;
    Piece piece;
    double value;
    int handle;

public:

    Field * position;
    
    Tile();
    Tile(const Ally& A, const Piece& piece, const int& h=0);
    
    const Ally& getAlly() const
    {
        return opponent;
    }
    const Piece& getPiece() const
    {
        return piece;
    }
    const char* getName() const
    {
        return TileNamesToText[static_cast<int>(piece)];
    }
    const int& getHandle() const
    {
        return handle;
    }
    double getValue() const;
    bool empty() const
    {
        return opponent == Ally::NONE;
    }
};

#endif // TILE_H
