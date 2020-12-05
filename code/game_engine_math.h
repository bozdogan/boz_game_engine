#ifndef GAME_ENGINE_MATH_H

float
Abs(float N)
{
    if(N >= 0) return N;
    else       return -N;
}

// NOTE(bora): 2D integer-based structs are mainly used for 
// screen-space operations.

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


v3
Normalized(v3 v)
{
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return v3{v.x/len, v.y/len, v.z/len};
}


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
DotProduct(v3 vec1, v3 vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

#define GAME_ENGINE_MATH_H
#endif