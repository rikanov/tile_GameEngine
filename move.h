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

#ifndef MOVE_H
#define MOVE_H

#include "field.h"
class Move : public Node
{
    Tile * revive;
public:
    Move():
    revive(Node::NONE)
    {
    }
    ~Move()
    {
    }
    
    void setRevive(Tile* t)
    {
        revive=t;
        t->position = nullptr;
    }
    int /*handle*/ getRevive() const
    {
        return revive->getHandle();
    }
    bool addToMarch(Field* f);
    void execute() const;
    void execute(Move * inv) const;
};

#endif // MOVE_H
