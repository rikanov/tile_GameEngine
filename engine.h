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

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "move.h"
#include "../View2D/view2d.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Engine: public Board
{
    static const int METRIC_BOUNDARY; 
    static const int DEPTH_BOUNDARY; 
    static const int MAX_NUMBER_OF_STEPS;
    
    BoardView * assigned_view;
    
    struct Position 
    {
        Piece crew;
        const int col;
        const int row;
    };
    static const Position StartPositions[];    
    struct Directions
    {
        Move * forward  = nullptr;
        Move * backward = nullptr;
    };
    int metric_space[96][96];
    std::vector<Directions> step_history;
    int step_index = 0;
    
    Ally current_turn;
    int searching_depth =0;
   
    Move * available_steps;
    Move * last_step;
    Move * current_step;
    Node * path;
    Field * start_field;
    
    bool isAllowedMove(const Move & m) const;
    bool compareToView() const;
    void swap()
    {
        current_turn = current_turn == Ally::OWN ? Ally::FOE : Ally::OWN;
        last_step = available_steps;
        getSteps();
    }
    
    Tile * tiles[32] = {};    // indexed by UI piece-handlers
    Tile * crew[2][16] = {};
 
    void getMarchingSteps();
    void getTeleporterSteps();
    void getHunterSteps();
    bool checkRange(Node* N, Field* step_to);
    void useTeleports();
    
    void getStepFromView(Move*) const;
    int /*handle*/ setViewFromStep(Move*) const;
    
    void setUI(BoardView * v);
    void initMetric();
    void start();
    
public:
    Engine(const Ally& A, BoardView* B);
    ~Engine();
    void getSteps();
    
    void doStep(const Move& m);
    void undoStep();
    void redoStep();
    
    void loop();
    void smoke(const bool& w = false);
    
private:

};

#endif // ENGINE_H
