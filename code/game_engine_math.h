#ifndef GAME_ENGINE_MATH_H

// NOTE(bora): 2D integer-based structs are mainly used for 
// screen space manipulations.

struct v2d
{
    int x;
    int y;
};


struct rect2d
{
    int x;
    int y;
    int w;
    int h;
};


struct triangle2d
{
    v2d p[3];
};


struct v3
{
    float x;
    float y;
    float z;
};


struct mat4x4
{
    float m[4][4] = {0};
};


void
MultiplyMatrixVector(v3 &i, v3 &o, mat4x4 &m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}


float
Abs(float N)
{
    if(N >= 0) return N;
    else       return -N;
}

#define GAME_ENGINE_MATH_H
#endif