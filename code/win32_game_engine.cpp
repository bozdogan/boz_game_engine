#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#define local_persist static


struct triangle
{
    v3 p[3];
};


struct mesh
{
    std::vector<triangle> tris;
};


struct Demo : game_engine
{
    mesh CubeMesh;
    mat4x4 matProj;

    v3 Camera = {0, 0, 0};

    float Theta;

    void Init() override
    {
        srand(time(0));

        std::vector<v2d> points;
        for(int i = 0; i < 420; ++i)
            points.push_back(v2d{rand() % ScreenWidth,  rand() % ScreenHeight});

        for(auto it = points.begin(); it != points.end(); ++it)
            DrawPoint(*it, color{255, 0, 0, 255});

        CubeMesh.tris = {
            // SOUTH
            {0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f},

            // EAST                                                      
            {1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f},

            // NORTH                                                     
            {1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f},

            // WEST                                                      
            {0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f},

            // TOP                                                       
            {0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f},

            // BOTTOM                                                    
            {1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f},
        };

        float zNear = 0.1f;
        float zFar = 1000.0f;
        float Fov = 90.0f;
        float AspectRatio = (float)ScreenHeight / (float)ScreenWidth;
        float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

        matProj.m[0][0] = AspectRatio * FovRad;
        matProj.m[1][1] = FovRad;
        matProj.m[2][2] = zFar / (zFar - zNear);
        matProj.m[3][2] = (-zFar * zNear) / (zFar - zNear);
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;

        printf("%d by %d\n", ScreenWidth, ScreenHeight);
    }

    void Update(double ElapsedTime) override
    {
        // Clear Screen
        Fill({0, 0, ScreenWidth, ScreenHeight}, color{0, 0, 0, 255});

        // Set up rotation matrices
        mat4x4 matRotZ, matRotX;
        Theta += 1.0f * ElapsedTime;

        // Rotation Z
        matRotZ.m[0][0] = cosf(Theta);
        matRotZ.m[0][1] = sinf(Theta);
        matRotZ.m[1][0] = -sinf(Theta);
        matRotZ.m[1][1] = cosf(Theta);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        // Rotation X
        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(Theta * 0.5f);
        matRotX.m[1][2] = sinf(Theta * 0.5f);
        matRotX.m[2][1] = -sinf(Theta * 0.5f);
        matRotX.m[2][2] = cosf(Theta * 0.5f);
        matRotX.m[3][3] = 1;
        
        // Draw Triangles
        for (auto tri : CubeMesh.tris)
        {
            triangle triProjected;
            triangle triTranslated;
            triangle triRotatedZ;
            triangle triRotatedZX;

            // Rotate in Z-Axis
            MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
            MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
            MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

            // Rotate in X-Axis
            MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
            MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
            MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

            // Offset into the screen
            triTranslated = triRotatedZX;
            triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
            triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
            triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

            // Calculate normal and stuff
            v3 normal;
            v3 line1, line2;

            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;
            
            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            // float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
            // normal.x /= l;
            // normal.y /= l;
            // normal.z /= l; // TODO(bora): clean this if the function works
            normal = Normalized(normal);

            if((normal.x * (triTranslated.p[0].x - Camera.x) +
                normal.y * (triTranslated.p[0].y - Camera.y) + 
                normal.z * (triTranslated.p[0].z - Camera.z) ) < 0)
            {
                v3 LightDirection = Normalized({0, 0, -1.0});
                Uint8 ColorValue = DotProduct(normal, LightDirection) * 255;

                // Project triangles from 3D --> 2D
                MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
                MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
                MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

                // Scale into view
                triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
                triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
                triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
                triProjected.p[0].x *= 0.5f * (float)ScreenWidth;
                triProjected.p[0].y *= 0.5f * (float)ScreenHeight;
                triProjected.p[1].x *= 0.5f * (float)ScreenWidth;
                triProjected.p[1].y *= 0.5f * (float)ScreenHeight;
                triProjected.p[2].x *= 0.5f * (float)ScreenWidth;
                triProjected.p[2].y *= 0.5f * (float)ScreenHeight;
            
                // Rasterize triangle
                FillTriangle(
                        triangle2d{
                            (int)triProjected.p[0].x, (int)triProjected.p[0].y,
                            (int)triProjected.p[1].x, (int)triProjected.p[1].y,
                            (int)triProjected.p[2].x, (int)triProjected.p[2].y},
                        color{ColorValue, ColorValue, ColorValue, 255});

                // DrawTriangle(
                //         triangle2d{
                //             (int)triProjected.p[0].x, (int)triProjected.p[0].y,
                //             (int)triProjected.p[1].x, (int)triProjected.p[1].y,
                //             (int)triProjected.p[2].x, (int)triProjected.p[2].y},
                //         color{0, 0, 0, 255});
            }
        }
    }
};


int main(int argc, char **argv)
{
    Demo game;
    if(!game.Construct(1000, 800, "TESTING THE GAME ENGINE: 3D RENDERING!!1"))
        game.Run();
    
    return 0;
}