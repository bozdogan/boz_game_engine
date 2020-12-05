#ifndef GAME_ENGINE_DRAW_H

#include "game_engine_base.h"
#include "game_engine_math.h"


// TODO(bora): CLEAN THIS MESS UP!
// ALL OF IT!!

float
Area(int x1, int y1, int x2, int y2, int x3, int y3) 
{
    return 0.5 * Abs((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)));
}


bool
Inside(triangle2d Tri, int x, int y)
{
    float ActualArea = Area(Tri.p[0].x, Tri.p[0].y, Tri.p[1].x, Tri.p[1].y, Tri.p[2].x, Tri.p[2].y);

    float Area1 = Area(         x,          y, Tri.p[1].x, Tri.p[1].y, Tri.p[2].x, Tri.p[2].y);
    float Area2 = Area(Tri.p[0].x, Tri.p[0].y,          x,          y, Tri.p[2].x, Tri.p[2].y);
    float Area3 = Area(Tri.p[0].x, Tri.p[0].y, Tri.p[1].x, Tri.p[1].y,          x,          y);

    return (ActualArea - (Area1 + Area2 + Area3)) < .01;
}


void
_FillFlatBottomTriangle(game_engine *Engine, v2d v1, v2d v2, v2d v3, color Color)
{
    if(v2.y != v3.y)
    {
        if(v1.y == v1.y)
            Swap(&v1, &v3);
        if(v1.y == v3.y)
            Swap(&v2, &v3);
    }

    if(v2.y != v3.y)
        printf("PANIC\n");

    float Slope1 = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
    float Slope2 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

    float LineX1 = v1.x;
    float LineX2 = v1.x;

    for(int LineY = v1.y; LineY <= v2.y; ++LineY)
    {
        Engine->DrawLine((int)LineX1, LineY, (int)LineX2, LineY, Color);
        LineX1 += Slope1;
        LineX2 += Slope2;
    }
}


void
_FillFlatTopTriangle(game_engine *Engine, v2d v1, v2d v2, v2d v3, color Color)
{
    if(v1.y != v2.y)
    {
        if(v2.y == v3.y)
            Swap(&v1, &v3);
        if(v1.y == v3.y)
            Swap(&v2, &v3);
    }

    if(v1.y != v2.y)
        printf("PANIK\n");

    float Slope1 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
    float Slope2 = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

    float LineX1 = v3.x;
    float LineX2 = v3.x;

    for(int LineY = v3.y; LineY > v1.y; --LineY)
    {
        Engine->DrawLine((int)LineX1, LineY, (int)LineX2, LineY, Color);
        LineX1 -= Slope1;
        LineX2 -= Slope2;
    }
}


void
game_engine::FillTriangle(triangle2d Tri, color Color)
{
    // for(int y = 0; y < ScreenHeight; ++y)
    // {
    //     for(int x = 0; x < ScreenWidth; ++x)
    //     {
    //         if(Inside(Tri, x, y))
    //             DrawPoint({x, y}, Color);
    //     }
    // }

    if(Tri.p[1].y < Tri.p[0].y)
        Swap(&Tri.p[1], &Tri.p[0]);

    if(Tri.p[2].y < Tri.p[0].y) 
        Swap(&Tri.p[2], &Tri.p[0]);

    if(Tri.p[2].y < Tri.p[1].y) 
        Swap(&Tri.p[2], &Tri.p[1]);

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
        v2d v4 = {
            (int)(Tri.p[0].x + ((float)(Tri.p[1].y - Tri.p[0].y) / (float)(Tri.p[2].y - Tri.p[0].y)) * (Tri.p[2].x - Tri.p[0].x)), 
            Tri.p[1].y};
        _FillFlatBottomTriangle(this, Tri.p[0], Tri.p[1],       v4, Color);
        _FillFlatTopTriangle(   this, Tri.p[1],       v4, Tri.p[2], Color);
    }

    // for(int y = Tri.p[0].y; y <= Tri.p[2].y; ++y)
    // {
    //     // 1. Find the intersections of the scan line with all edges of the polygon.

    //     // 2. Sort the intersections by increasing x-coordinate.
        
    //     // 3. Fill in all pixels between pairs of intersections.

    // }

}

#define GAME_ENGINE_DRAW_H
#endif