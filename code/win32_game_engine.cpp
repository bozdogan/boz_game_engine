#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#define local_persist static


struct triangle
{
    v3f p[3];
};


struct mesh
{
    std::vector<triangle> tris;
};


struct Demo : game_engine
{
    mesh CubeMesh;
    mat4x4 matProj;

    v3f Camera = {0, 0, 0};

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
            {0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f},

            // EAST                                                      
            {1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f},

            // NORTH                                                     
            {1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f},

            // WEST                                                      
            {0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},

            // TOP                                                       
            {0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},

            // BOTTOM                                                    
            {1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f},
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

        mat4x4 WorldTransform;
        WorldTransform.m[0][0] = 1.0f;
        WorldTransform.m[1][1] = 1.0f;
        WorldTransform.m[2][2] = 1.0f;
        WorldTransform.m[3][3] = 1.0f;

        mat4x4 Translation;
        Translation.m[0][0] = 1.0f;
        Translation.m[1][1] = 1.0f;
        Translation.m[2][2] = 1.0f;
        Translation.m[3][3] = 1.0f;
        Translation.m[3][0] = 0;
        Translation.m[3][1] = 0;
        Translation.m[3][2] = 3;

        WorldTransform = Matrix_MultiplyMatrix(matRotZ, matRotX);
        WorldTransform = Matrix_MultiplyMatrix(WorldTransform, Translation);
        
        // Draw Triangles
        for (auto tri : CubeMesh.tris)
        {
            triangle triProjected;
            triangle triTranslated;
            triangle triRotatedZ;
            triangle triRotatedZX;

            // Rotate and offset into the screen
            triTranslated.p[0] = Matrix_MultiplyVector(WorldTransform, tri.p[0]);
            triTranslated.p[1] = Matrix_MultiplyVector(WorldTransform, tri.p[1]);
            triTranslated.p[2] = Matrix_MultiplyVector(WorldTransform, tri.p[2]);

            // Calculate normal and stuff
            v3f normal;
            v3f line1, line2;

            line1 = Vector_Sub(triTranslated.p[1], triTranslated.p[0]);

            line2 = Vector_Sub(triTranslated.p[2], triTranslated.p[0]);

            normal = Vector_CrossProduct(line1, line2);

            normal = Vector_Normalize(normal);

            if(Vector_DotProduct(normal, Vector_Sub(triTranslated.p[0], Camera)) < 0)
            {
                v3f LightDirection = Vector_Normalize({0, 0, -1.0});
                Uint8 ColorValue = Vector_DotProduct(normal, LightDirection) * 255;

                // Project triangles from 3D --> 2D
                triProjected.p[0] = Matrix_MultiplyVector(matProj, triTranslated.p[0]);
                triProjected.p[1] = Matrix_MultiplyVector(matProj, triTranslated.p[1]);
                triProjected.p[2] = Matrix_MultiplyVector(matProj, triTranslated.p[2]);

                // Scale into view
                triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

                triProjected.p[0].x += 1.0f;
                triProjected.p[0].y += 1.0f;
                triProjected.p[1].x += 1.0f;
                triProjected.p[1].y += 1.0f;
                triProjected.p[2].x += 1.0f;
                triProjected.p[2].y += 1.0f;
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