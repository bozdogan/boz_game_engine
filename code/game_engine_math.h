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


//
// 3D VECTOR
//

struct v3f
{
    float x;
    float y;
    float z;
};


float
Vector_Length(v3f v)
{
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}


float
Vector_DotProduct(v3f v1, v3f v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


v3f
Vector_Normalize(v3f v)
{
    float len = Vector_Length(v);
    return {v.x/len, v.y/len, v.z/len};
}


v3f
Vector_Add(v3f v1, v3f v2)
{
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}


v3f
Vector_Sub(v3f v1, v3f v2)
{
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}


v3f
Vector_Mul(v3f v1, float k)
{
    return {v1.x*k, v1.y*k, v1.z*k};
}


v3f
Vector_Div(v3f v1, float k)
{
    return {v1.x/k, v1.y/k, v1.z/k};
}


v3f
Vector_CrossProduct(v3f v1, v3f v2)
{
    v3f v;
    v.x = v1.y*v2.z - v1.z*v2.y;
    v.y = v1.z*v2.x - v1.x*v2.z;
    v.z = v1.x*v2.y - v1.y*v2.x;
    return v;
}


struct mat4x4
{
    float m[4][4] = {0};
};


void
MultiplyMatrixVector(v3f &i, v3f &o, mat4x4 &m)
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

#define GAME_ENGINE_MATH_H
#endif