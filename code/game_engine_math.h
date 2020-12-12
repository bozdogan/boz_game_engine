#ifndef GAME_ENGINE_MATH_H

#define PI_F 3.14159f

float
Abs(float N)
{
    if(N >= 0) return N;
    else       return -N;
}

// NOTE(bora): 2D integer-based structs are mainly used for 
// screen-space operations.

struct v2i
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
    v2i p[3];
};


//
// 3D VECTOR
//

struct v3f
{
    float x;
    float y;
    float z;

    // NOTE(bora): Needs an additional term to work with 4 by 4 matrices.
    float w = 1;
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
Vector_Normalize(v3f v)
{
    float len = Vector_Length(v);
    return {v.x/len, v.y/len, v.z/len};
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


v3f
Matrix_MultiplyVector(mat4x4 m, v3f v)
{
    v3f Vector;
    Vector.x = v.x*m.m[0][0] + v.y*m.m[1][0] + v.z*m.m[2][0] + v.w*m.m[3][0];
    Vector.y = v.x*m.m[0][1] + v.y*m.m[1][1] + v.z*m.m[2][1] + v.w*m.m[3][1];
    Vector.z = v.x*m.m[0][2] + v.y*m.m[1][2] + v.z*m.m[2][2] + v.w*m.m[3][2];
    Vector.w = v.x*m.m[0][3] + v.y*m.m[1][3] + v.z*m.m[2][3] + v.w*m.m[3][3];
    
    return Vector;
}


mat4x4 
Matrix_MultiplyMatrix(mat4x4 m1, mat4x4 m2)
{
    mat4x4 Matrix;
    for (int Col = 0; Col < 4; ++Col)
    for (int Row = 0; Row < 4; ++Row)
        Matrix.m[Row][Col] = (m1.m[Row][0]*m2.m[0][Col] + 
                              m1.m[Row][1]*m2.m[1][Col] + 
                              m1.m[Row][2]*m2.m[2][Col] + 
                              m1.m[Row][3]*m2.m[3][Col]);

    return Matrix;
}

#define GAME_ENGINE_MATH_H
#endif