/*
 * Simple basic data structure to store and manage pointers up to 4 object
 * This is the base class for the Field and Move classes
 *
 * 
 * Copyright (C) 2018  Robert Ikanov <robert.ikanov@gmail.com>
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

#ifndef CONNECTION_HEADER__
#define CONNECTION_HEADER__

#include"enums.h"
#include"tile.h"

class Field;
class Node
{
friend class Board;
friend class Engine;
protected:  
    
    static Tile * NONE;
    
    int current_size;
    mutable 
    Field ** progress_pointer;
    Field ** connections;
    Field ** stack_pointer;
    Field ** end_pointer;
    Field ** inner_store;
    
public:
    
    int value = 0;
    
    Node():
     inner_store(new Field* [5])
    {
        reinit();
    }
    Node(const int& max_storage):
     inner_store(new Field* [max_storage+1])
    {
        reinit();
    }
    virtual ~Node()
    {
        progress_pointer = stack_pointer = connections = end_pointer = nullptr;
        delete[] inner_store;
        inner_store = nullptr;
    }
    void getConnectionsFrom(const Node * from)
    {
        progress_pointer = from->connections;
        connections      = from->connections;
        stack_pointer    = from->stack_pointer;
        end_pointer      = from->end_pointer;
        current_size     = from->current_size;
    }
    void reinit()
    {
        current_size = 0;
        end_pointer = inner_store;
        progress_pointer = stack_pointer = connections = end_pointer +1;
    }
    void clear()
    {
        stack_pointer = progress_pointer = connections;
        current_size = 0;
    }
    bool notEnded() const
    {
        return progress_pointer != stack_pointer;
    }
    bool inProgress() const
    {
        return progress_pointer != connections;
    }
    void reset()
    {
        progress_pointer = connections;
    }
    bool single() const
    {
        return current_size == 0;
    }
    const Field * end() const
    {
        return *end_pointer;
    }
    void begin()
    {
        progress_pointer = end_pointer;
    }
    Field * start() const
    {
        progress_pointer = connections;
        return curr();
    }
    const int& size() const
    {
        return current_size;
    }
    Field * curr() const
    {
        return (progress_pointer != stack_pointer) ? *progress_pointer : *end_pointer;
    }
    Field * next() const
    {
        progress_pointer += (progress_pointer != stack_pointer);
        return curr();
    }
    Field * prev() const
    {
        return (progress_pointer != end_pointer) ? *(--progress_pointer) : *end_pointer;
    }
    Field * last() const
    {
        return current_size > 0 ? *(stack_pointer-1) : *end_pointer;
    }
    Field * blast() const
    {
        return current_size > 1 ? *(stack_pointer-2) : *end_pointer;
    }
    Field * at(const int& index) const
    {
        return index < current_size ? *(connections+index) : *end_pointer;
    }
    void bind(Field * next)
    {
        *(stack_pointer++) = next;
        ++current_size;
    }
    void append(const Node * N)
    {
        for(Field* n=N->start(); N->curr() != N->end(); n = N->next())
        {
            bind(n);
        }
    }
    bool imbue(Field * next)
    {
        for(Field ** n = connections; n != stack_pointer; ++n)
        {
            if(*n == next)
            {
                return false;
            }
        }
        bind(next);
        return true;
    }
    void pop()
    {
        if(current_size > 0)
        {
            --current_size;
            --stack_pointer;
        }
    }
    Field * back() const
    {
        progress_pointer = stack_pointer - 1;
        return *progress_pointer;
    }
    bool operator == (const Node& n) const;
    Field * find(Field * n) const;
};

#endif
