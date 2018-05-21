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

#include "field.h"

 
void Field::initTeleports()
{
    teleports = new Node(6);
    for(start(); notEnded(); next())
    {
        for(Field * n = curr()->start(); curr()->notEnded(); n = curr()->next())
        {
            if(n != this)
            {
                teleports->bind(n);
            }
        }
    }
}

void Field::initEnvironment()
{
    environment = new Node(20);
    environment->bind(this);
    value = 0;
    for(Node * next = environment->start(); environment->notEnded(); next = environment->next())
    {
        if(next->value == 3)
        {
            continue;
        }
        for(next->start(); next->notEnded(); next->next())
        {
            if(environment->imbue(next->curr()))
            {
                next->curr()->value = next->value +1;
            }
        }
    }
    for(environment->start(); environment->notEnded(); environment->next())
    {
        environment->curr()->value = 0;
    }
}

int Field::getDef() const
{
    if( tile->getPiece() == Piece::NONE)
    {
        return -1;
    }
    const Ally A = getAlly();
    int result = isDefender(getPiece());
    if(result)
    {
        for(start(); curr() != end(); next())
        {
            result += curr()->getDef(A);
        }
    }
    return result;
}

bool Field::defiance(const Ally& attacker, const int& power) const
{
    if(tile->getAlly() != attacker)
    {
        return getDef() >= power;
    }
    else
    {
        return true;
    }
}

void Field::setPosition(const int& c, const int& r)
{
    if(!initialized)
    {
        col = c;
        row = r;
        initialized = true;
    }
}
