#ifndef GAME_ENGINE_DRAW_H

#include "game_engine_base.h"
#include "game_engine_math.h"

void
_FillFlatBottomTriangle(game_engine *Engine, v2i v1, v2i v2, v2i v3, color Color)
{
    if(v2.y != v3.y)
    {
        if(v1.y == v1.y)
            Swap(&v1, &v3);
        if(v1.y == v3.y)
            Swap(&v2, &v3);
    }

    if(v2.y != v3.y)
        // TODO(bora): Something's got terribly wrong! I should probably log this.
        // Here is a helpful print statement for now.
        printf("PANIC\n");

    float Slope1 = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
    float Slope2 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

    float xLeft = v1.x;
    float xRight = v1.x;

    for(int y = v1.y; y <= v2.y; ++y)
    {
        Engine->DrawLine((int)xLeft, y, (int)xRight, y, Color);
        xLeft += Slope1;
        xRight += Slope2;
    }
}


void
_FillFlatTopTriangle(game_engine *Engine, v2i v1, v2i v2, v2i v3, color Color)
{
    if(v1.y != v2.y)
    {
        if(v2.y == v3.y)
            Swap(&v1, &v3);
        if(v1.y == v3.y)
            Swap(&v2, &v3);
    }

    if(v1.y != v2.y)
        // TODO(bora): Something's got terribly wrong! I should probably log this.
        // Here is a helpful print statement for now.
        printf("PANIK\n");

    float Slope1 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
    float Slope2 = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

    float xLeft = v3.x;
    float xRight = v3.x;

    for(int y = v3.y; y > v1.y; --y)
    {
        Engine->DrawLine((int)xLeft, y, (int)xRight, y, Color);
        xLeft -= Slope1;
        xRight -= Slope2;
    }
}


void
game_engine::FillTriangle(triangle2d Tri, color Color)
{
    // NOTE(bora): Basically what I do here is sort vertices so they are in 
    // ascending order by their y-components and try to determine if one of 
    // the edges is parallel to x-axis. If so, start at the point-side and
    // fill each line until flat-side is reached. Otherwise, split the triangle
    // so there are two triangles with one flat side and to exactly the same thing.
    // It seems to work for now!

    // TODO(bora): See if there is a better approach than my shitty algorithm.

    if(Tri.p[1].y < Tri.p[0].y) Swap(&Tri.p[1], &Tri.p[0]);
    if(Tri.p[2].y < Tri.p[0].y) Swap(&Tri.p[2], &Tri.p[0]);
    if(Tri.p[2].y < Tri.p[1].y) Swap(&Tri.p[2], &Tri.p[1]);

    if (Tri.p[1].y == Tri.p[2].y)
    {
        _FillFlatBottomTriangle(this, Tri.p[0], Tri.p[1], Tri.p[2], Color);
    }
    else if (Tri.p[0].y == Tri.p[1].y)
    {
        _FillFlatTopTriangle(this, Tri.p[0], Tri.p[1], Tri.p[2], Color);
    }
    else
    {
        v2i v4 = {
            (int)(Tri.p[0].x + ((float)(Tri.p[1].y - Tri.p[0].y) / (float)(Tri.p[2].y - Tri.p[0].y)) * (Tri.p[2].x - Tri.p[0].x)), 
            Tri.p[1].y};
        _FillFlatBottomTriangle(this, Tri.p[0], Tri.p[1],       v4, Color);
        _FillFlatTopTriangle(   this, Tri.p[1],       v4, Tri.p[2], Color);
    }
}

#define GAME_ENGINE_DRAW_H
#endif