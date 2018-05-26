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
#include <iostream>
 
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

void Field::initRangedSpots()
{
    std::cout<<"col: "<<col<<" row: "<<row<<std::endl;
    for(int i = 0, j = 0; i<size(); ++i)
    {
        Field * next = at(i);
        Node * r = new Node;
        ranged_spots[i][0] = r;
        r->bind(next);
        for(next->start(); next->notEnded(); next->next())
        {
            if(next->curr() != this)
            {
                r->bind(next->curr());
            }
        }
        Node * q = new Node;
        ranged_spots[i][1] = q;
        q->bind(r->at(1));
        for(Field * p = r->at(1)->start(); r->at(1)->notEnded(); p = r->at(1)->next())
        {
            if(p != next)
            {
                q->bind(p);
            }
        }
        if(r->at(2) == r->end())
        {
            continue;
        }
        q->bind(r->at(2));
        //..........
        q = new Node;
        ranged_spots[i][2] = q;
        q->bind(r->at(2));
        q->bind(r->at(1));        
        for(Field * p = r->at(2)->start(); r->at(2)->notEnded(); p = r->at(2)->next())
        {
            if(p != next)
            {
                q->bind(p);
            }
        }
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
