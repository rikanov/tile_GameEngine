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

bool Engine::inNeighbourhood(Move* m1, Move* m2) const
{
    if(metric_space[m1->last()->getMetric()][m2->last()->getMetric()] < 3 || metric_space[m1->last()->getMetric()][m2->first()->getMetric()] < 3 ||
       metric_space[m1->first()->getMetric()][m2->first()->getMetric()] < 3 || metric_space[m1->first()->getMetric()][m2->last()->getMetric()] < 3)
    {
        return true;
    }
    else
    {
        return false;
    } /*
    for(m1->start(); m1->notEnded(); m1->next())
    {
        for(m2->start(); m2->notEnded(); m2->next())
        {
            if(metric_space[m1->curr()->getMetric()][m2->curr()->getMetric()] <= 2)
            {
                return true;
            }
        }
    }
    return false; //*/
}

double Engine::evaluate() const
{
    double result[2] = {0.0 , 0.0};
    
    for(int ally=0; ally < 2; ++ally)
    {
        for(int index = 0; index < 16; ++index)
        {
            result[ally] += crew[ally][index]->getValue();
        }
    }
    return current_turn == Ally::OWN ? result[1] / result[0] : result[0] / result[1];
}

Move * Engine::chooseStep()
{
    Move * check, *result;
    double max = -1.0, next;
    for(check = available_steps; check != last_step; ++check)
    {
        check->execute(current_step);
        next = evaluate();
        if(next > max)
        {
            max = next;
            result = check;
        }
        current_step->execute();
    }
    result->evaluated = 1/max;
    return result;
}

Move * Engine::findStepFor(Move* last)
{
    Move  *result = nullptr;
    if(searching_depth == AI_LEVEL)
    {
        current_step->evaluated = 1/evaluate();
        return current_step;
    }
    ++searching_depth;
    getSteps();
    Move back, *next;
    double max = 0.0;
    for(Move * check = deep_search[searching_depth]; check != end_search[searching_depth]; ++check)
    {
        if(last->last()->empty() == false /*hit*/ || inNeighbourhood(last, check))
        {
            check->execute(&back);
            next = findStepFor(check);
            if(next->evaluated > max)
            {
                max = next->evaluated;
                result = check;
            }
            back.execute();
        }
    }
    result->evaluated = 1/max;
    available_steps = deep_search[--searching_depth];
    return result;
}

void Engine::AI()
{
    searching_depth = 0;
    available_steps = deep_search[0];
    getSteps();
    double max = 0.0;
    Move back, *next, *result = nullptr;
    for(Move* check = available_steps; check != end_search[0]; ++check)
    {
        check->execute(&back);
        next = findStepFor(check);
        if(next->evaluated > max)
        {
            max = next->evaluated;
            result = check;
        }
        back.execute();
    }
    doStep(result);
}

