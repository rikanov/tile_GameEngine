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

#include "move.h"
#include <iostream>

/*Move::Move():
 revive(new Field)
{
    revive->tile = NONE;
    *end_pointer = revive;
}
*/

void Move::execute() const
{
    for(Field * f = back(); f != *connections; f = curr())
    {
        f->setTile(prev());
    }
    at(0)->setTile(revive);
}

void Move::execute(Move* inv) const
{    
    inv->clear();
    inv->setRevive(last()->getTile());
    for(Field * f = back(); f != *connections; f = curr())
    {
        f->setTile(prev());
        inv->bind(f);
    }
    at(0)->setTile(revive);
    inv->bind(at(0));
}

bool Move::addToMarch(Field * f)
{
    switch(size())
    {
        case 1:
            if(f->getAlly() == last()->getAlly() && isMarchingGroup(at(0)->getPiece(),f->getPiece()))
            {
                bind(f);
                return true;
            }
            return false;
        case 2:
            if(f->getAlly() == last()->getAlly() && f != at(0) && isMarchingGroup(at(0)->getPiece(),at(1)->getPiece(),f->getPiece()))
            {
                bind(f);
                return true;
            }
            return false;
        default:
            return false;            
    }
    return false;
}
