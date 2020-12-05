#ifndef GAME_ENGINE_H

#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif
#include <chrono>

#include "game_engine_math.h"


struct color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};


bool
ColorMatches(color Color1, color Color2)
{
    return (Color1.r == Color2.r && 
            Color1.g == Color2.g && 
            Color1.b == Color2.b &&
            Color1.a == Color2.a);
}


void
Swap(v2d *p1, v2d *p2)
{
    v2d temp = *p2;
    *p2 = *p1;
    *p1 = temp;
}


#include "game_engine_base.h"
#include "game_engine_draw.h"

#define GAME_ENGINE_H
#endif