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

#ifndef FIELD_H
#define FIELD_H

#include "node.h"
#include "tile.h"

class Field: public Node
{
public:
    static const int MAX_DEPTH = 10;
   
protected:
    
    Tile * tile;
    
    int col = -1, row = -1;
    bool initialized = false;
    
    Node * ranged_spots[4][4] = {};
    
public:
    double ai_score = 0.0;
    
    Field()
    {
    }
    virtual ~Field()
    {
        for(int r = 0; r < 4; ++r)
            for(int q = 0; q < 4; ++q)
            {
                delete ranged_spots[r][q];
            }
    }
    void initRangedSpots();
    
    void setPosition(const int& c, const int& r);
    
    const int& getCol() const
    {
        return col;
    }
    const int& getRow() const
    {
        return row;
    }
    void setTile(Tile *t)
    {
        tile = t;
        t->position = this;
    }
    void setTile(Field * f)
    {
        tile = f->tile;
        f->tile->position = this;
    }
    bool defiance(const Ally& attacker, const int& power) const;
    Tile * getTile() const
    {
        return tile;
    }    const Ally& getAlly() const
    {
        return tile->getAlly();
    }
    const Piece& getPiece() const
    {
        return tile->getPiece();
    }
    const Piece getPiece(const Ally& A) const
    {
        return getAlly() == A ? tile->getPiece() : Piece::NONE;
    }
    int getDef() const;
    int getDef(const Ally& A ) const
    {
        return isDefender(getPiece(A));
    }
    const char * getName() const
    {
        return tile ? tile->getName() : "not defined";
    }
    bool empty() const
    {
        return tile->empty();
    }
    const auto getRange() const
    {
        return ranged_spots;
    }
    static void connect(Field * n1, Field * n2)
    {
        n1->bind(n2);
        n2->bind(n1);        
    }
};

#endif // FIELD_H
