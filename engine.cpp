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

#include <iostream>
#include "engine.h"
#include "tile.h"
#include "field.h"

const int Engine::DEPTH_BOUNDARY = 20;
const int Engine::MAX_NUMBER_OF_STEPS = 1000;

const Engine::Position Engine::StartPositions[] =
{
    { Piece::HUNTER      , 1, 0},
    { Piece::HUNTER      , 2, 0},
    { Piece::HUNTER      , 3, 0},
    { Piece::HUNTER      , 3, 1},
    { Piece::WARRIOR     , 4, 0},
    { Piece::ASSASSIN    , 4, 1},
    { Piece::GUARDIAN    , 6, 1},
    { Piece::AURADIN     , 5, 0},
    { Piece::AURADIN     , 6, 0},
    { Piece::AURADIN     , 7, 0},
    { Piece::AURADIN     , 7, 1},    
    { Piece::TELEPORTER  , 5, 2},
    { Piece::TELEPORTER  , 6, 2},
    { Piece::TELEPORTER  , 7, 2},
    { Piece::TELEPORTER  , 7, 3},
    { Piece::COMMANDER   , 5, 1},
};

Engine::Engine(const Ally& A, BoardView* B)
:Board()
,current_turn(A)
,assigned_view(nullptr)
,available_steps(new Move[200])
,current_step(new Move)
,path(new Node)
{
    step_history.resize(MAX_NUMBER_OF_STEPS);
    start();
    setUI(B);
}

void Engine::start()
{
    std::cout<<"Engine start"<<std::endl;
    int index = 0, member = 0;
    for(Position p : StartPositions)
    {
        // set enemies first 
        Tile * next = new Tile(Ally::FOE,p.crew,index);
        setTile(next,p.col, p.row);
        tiles[index++] = crew[0][member] = next;
        
        // set own army on the opposite side of board
        next = new Tile(Ally::OWN, p.crew,index);
        setTile(next, 8 + 2 * p.row - p.col, 7 - p.row);
        tiles[index++] = crew[1][member++] = next;
    }
    std::cout<<"Engine has started"<<std::endl;
}

void Engine::setUI(BoardView* v)
{
    std::cout<<"Setting UI"<<std::endl;
    assigned_view = v;
    for(Tile * t: tiles)
    {
        const Field* p = t->position;
        assigned_view->createPieces(t->getAlly(),p->getCol(),p->getRow(),t->getName());
    }
    std::cout<<"UI has set"<<std::endl;
}

bool Engine::compareToView() const
{
    for(int index=0;index<32;++index)
    {
        if(tiles[index]->position == nullptr)
        {
            continue;
        }
        const Field * p = tiles[index]->position;
        if(index + 1 != assigned_view->getHandle(p->getCol(),p->getRow()))
        {
            std::cerr << "mismatch error: " << p->getCol() << ':' << p->getRow() << ' ' << (index+1) << " != " << assigned_view->getHandle(p->getCol(),p->getRow()) << std::endl;
            return false;
        }
    }
    std::cout<<"compare Ok. " <<std::endl;
    return true;
}

void Engine::getSteps()
{
    last_step = available_steps;
    last_step->clear();
    const int A = current_turn == Ally::OWN;
    for(int index = 0; index<16; ++index)
    {
        start_field = crew[A][index]->position;
        if(start_field)
        {
            useTeleports();
            const Piece P = crew[A][index]->getPiece();
            if(isAttacker(P))
            {
                getMarchingSteps();
            }
            if(isTeleporter(P))
            {
                getTeleporterSteps();
            }
            if(isHunter(P))
            {
               getHunterSteps();
            }
        }
    }
}

void Engine::getMarchingSteps()
{
    current_step->clear();
    current_step->bind(start_field);
    for(current_step->last()->begin(); current_step->single()==false; current_step->pop())  // check and drop the last field in the queue
    {
        for(current_step->last()->next();current_step->last()->notEnded();current_step->last()->next()) // look around on the last field
        {
            while(current_step->size()<3 && current_step->addToMarch(current_step->last()->curr()))  // get brothers in arms for marching
            {
                current_step->last()->start();
            }
            if(current_step->last()->curr()->defiance(current_turn,current_step->size())==false)
            {
                last_step->clear();
                last_step->setRevive(Node::NONE);
                last_step->append(current_step);
                last_step->bind(current_step->last()->curr());
                ++last_step;
            }
        }
    }
}

void Engine::getTeleporterSteps()
{
    if(isAttacker(start_field->getPiece())==false) // avoid some redundancy
    {
        for(start_field->start(); start_field->notEnded(); start_field->next())
        {
            if(start_field->curr()->empty())
            {
                last_step->clear();
                last_step->bind(start_field);
                last_step->bind(start_field->curr());
                ++last_step;                
            }
        }
    }
    Node * t = start_field->getTeleports();
    for(Field * n = t->start(); t->notEnded(); n = t->next())
    {
        if(n->empty())
        {
            last_step->clear();
            last_step->bind(start_field);
            last_step->bind(n);
            ++last_step;
        }
    }
}

void Engine::getHunterSteps()
{
    for(Field * f = start_field->start(); start_field->notEnded(); f = start_field->next())
    {
        if(f->empty() == false)
        {
            continue;
        }
        const auto range = f->getRange(); 
        for(int neighbour = 0; neighbour < f->size(); ++neighbour)
        {
            auto N = range[neighbour];
            if(checkRange(N[0],f))
            {
                continue;
            }
            checkRange(N[1],f);
            if(N[2] != nullptr)
            {
                checkRange(N[2],f);
            }
        }        
    }
}

bool /*is closed?*/ Engine::checkRange(Node* N, Field* step_to)
{
    if(N->start()->empty() == false && N->curr() != start_field)
    {
        return true;
    }
        
    int attack_rate = 1;
    for(Field * h = N->next(); N->notEnded(); h = N->next())
    {
        if(h->empty())
        {
            attack_rate = 0;
            break;
        }
        else
        {
            attack_rate += isHunter(h->getPiece(current_turn));
        }
    }
    for(Field * h = N->start(); N->notEnded(); h = N->next())
    {
        if(h->defiance(current_turn, attack_rate) == false)
        {
            last_step->clear();
            last_step->bind(start_field);
            last_step->bind(step_to);
            last_step->bind(h);
            ++last_step;
        }
    }
    return attack_rate != 0;
}


void Engine::doStep(const Move& m)
{
    Move * frwrd = new Move;
    Move * bcwrd = new Move;
    frwrd->append(&m);
    frwrd->execute(bcwrd /*get inverse*/);
    step_history[step_index].forward = frwrd;
    ++step_index;
    delete step_history[step_index].forward;
    delete step_history[step_index].backward;
    step_history[step_index].backward = bcwrd;
}

void Engine::useTeleports()
{
    current_step->clear();
    current_step->bind(start_field);
    for(current_step->last()->begin(); current_step->single()==false; current_step->pop())  // check and drop the last field in the queue
    {
        for(current_step->last()->next();current_step->last()->notEnded();current_step->last()->next()) // look around on the last field
        {
            Field * next = current_step->last()->curr();
            while(current_step->size()<4 && isTeleporter(next->getPiece()) && next->getAlly() == current_turn) // a new teleporter unit can be joined to chain
            {
                if(current_step->blast() == next) // we've turned back
                {
                    break;
                }
                current_step->bind(next);
                current_step->last()->start();
                next = current_step->last()->curr(); // refresh
            }
            if(next->empty())
            {
                last_step->clear();
                last_step->bind(start_field);
                last_step->bind(next);
                ++last_step;
            }
        }
    }
}

void Engine::getStepFromView(Move* n) const
{
    n->clear();
    for(SDL_Point p : assigned_view->selected)
    {
        n->bind(board[p.x][p.y]);
    }
}

int /*handle*/ Engine::setViewFromStep(Move* n) const
{
    assigned_view->selected.clear();
    for(n->start(); n->notEnded(); n->next())
    {
        SDL_Point next;
        next.x = n->curr()->getCol();
        next.y = n->curr()->getRow();
        assigned_view->selected.push_back(next);
    }
    return assigned_view->moveSelection(n->getRevive()+1);
}

bool Engine::isAllowedMove(const Move& m) const
{
    for(Move* check = available_steps; check != last_step; ++check)
    {
        if( *check == m)
        {
            return true;
        }
    }
    return false;
}

void Engine::loop()
{
    Move next;
    last_step = available_steps;
    getSteps();
    while(assigned_view->exit_request == false)
    {
        assigned_view->select();
        if(assigned_view->undo_request && step_index)
        { 
            step_history[step_index].backward->execute();
            setViewFromStep(step_history[step_index--].backward);
            swap();
            //compareToView();
            continue;
        }
        if(assigned_view->redo_request && step_history[step_index].forward)
        {
            step_history[step_index].forward->execute();
            setViewFromStep(step_history[step_index++].forward);
            swap();
            //compareToView();
            continue;                
        }
        getStepFromView(&next); 
        if(isAllowedMove(next))
        {
            doStep(next);
            setViewFromStep(&next);
            swap();
            //compareToView();
        }
        assigned_view->selected.clear();
    }
}

Engine::~Engine()
{
    std::cout << "Stop engine... " << std::endl;
    std::cout<<"Delete tiles ... "<<std::endl;
    for(Tile * t: tiles)
    {
        delete t;
    }
    std::cout<<"Done. "<<std::endl;
    std::cout<<"purify step history..."<<std::endl;
    for(step_index=0; step_index<MAX_NUMBER_OF_STEPS; ++step_index)
    {
        delete step_history[step_index].forward;
        delete step_history[step_index].backward;
    }
    std::cout<<"Done. "<<std::endl;
    delete assigned_view;
    delete current_step;
    delete path;
    delete[] available_steps;
    std::cout <<"Running engine has been finished."<<std::endl;
}
