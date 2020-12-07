
#ifndef GAME_ENGINE_3D_RENDER_H

#include "game_engine_math.h"
#include <cmath>

mat4x4
MakeMatrix_Projection(
        float zNear,
        float zFar,
        float Fov,
        float AspectRatio)
{
    float FovRad = 1.0f/tanf(0.5f*Fov / 180.0f * PI_F);

    mat4x4 matrix;
    matrix.m[0][0] = AspectRatio * FovRad;
    matrix.m[1][1] = FovRad;
    matrix.m[2][2] = zFar / (zFar - zNear);
    matrix.m[3][2] = (-zFar * zNear) / (zFar - zNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;

    return matrix;
}


mat4x4
MakeMatrix_RotX(float Theta)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1;
    matrix.m[1][1] = cosf(Theta * 0.5f);
    matrix.m[1][2] = sinf(Theta * 0.5f);
    matrix.m[2][1] = -sinf(Theta * 0.5f);
    matrix.m[2][2] = cosf(Theta * 0.5f);
    matrix.m[3][3] = 1;

    return matrix;
}


mat4x4
MakeMatrix_RotZ(float Theta)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(Theta);
    matrix.m[0][1] = sinf(Theta);
    matrix.m[1][0] = -sinf(Theta);
    matrix.m[1][1] = cosf(Theta);
    matrix.m[2][2] = 1;
    matrix.m[3][3] = 1;

    return matrix;
}


mat4x4
MakeMatrix_Translation(float x, float y, float z)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;

    return matrix;
}

#define GAME_ENGINE_3D_RENDER_H
#endif