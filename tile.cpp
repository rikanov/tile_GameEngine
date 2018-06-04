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

#include "tile.h"
#include "node.h"
#include "field.h"

double Tile::value_norm = 0.0;
const double Tile::StandardValues[] =
{
    0.0,    // some kind of error
    1.0,    // auradin
    1.0,    // hunter
    2.5,    // warrior
    1.0,    // teleporter
    2.2,    // guardian
    2.3,    // assassin
    3.5     // commander
};

const char* Tile::TileNamesToText[] = 
{
    "none",    // some kind of error
    "auradin",
    "hunter",
    "warrior",
    "teleporter",
    "guardian",
    "assassin",
    "commander"
};

Tile::Tile()
 :opponent(Ally::NONE)
 ,piece(Piece::NONE)
 ,handle(-1)
{
    
}
Tile::Tile(const Ally& A, const Piece& piece, const int& h)
 :opponent(A)
 ,piece(piece)
 ,handle(h)
{
    if(value_norm==0.0)
    {
        for(double d : StandardValues)
        {
            value_norm += d;
        }
    }
    value = StandardValues[static_cast<int>(piece)] / value_norm;
}
double Tile::getValue() const
{
    return position == nullptr ? 0 : value * position->ai_score;
}
