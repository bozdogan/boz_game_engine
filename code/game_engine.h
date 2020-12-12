#ifndef GAME_ENGINE_H

#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#define local_persist static

#include "game_engine_math.h"
#include "game_engine_base.h"
#include "game_engine_draw.h"
#include "game_engine_3d_render.h"

#define GAME_ENGINE_H
#endif