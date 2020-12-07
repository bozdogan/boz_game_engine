#include "game_engine.h"
#include <cstdlib>
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


triangle2d
ExtractTriXY(triangle Tri)
{
    return triangle2d{
            (int)Tri.p[0].x, (int)Tri.p[0].y,
            (int)Tri.p[1].x, (int)Tri.p[1].y,
            (int)Tri.p[2].x, (int)Tri.p[2].y};
}


struct Demo : game_engine
{
    mesh CubeMesh;
    mat4x4 Projection;

    v3f Camera = {0, 0, 0};
    float Theta;

    void Init() override
    {
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

        // float zNear = 0.1f;
        // float zFar = 1000.0f;
        // float Fov = 90.0f;
        // float AspectRatio = (float)ScreenHeight / (float)ScreenWidth;
        // float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

        // Projection.m[0][0] = AspectRatio * FovRad;
        // Projection.m[1][1] = FovRad;
        // Projection.m[2][2] = zFar / (zFar - zNear);
        // Projection.m[3][2] = (-zFar * zNear) / (zFar - zNear);
        // Projection.m[2][3] = 1.0f;
        // Projection.m[3][3] = 0.0f;
        Projection = MakeMatrix_Projection(
                0.1f, 1000.0f,
                90.0f,
                (float)ScreenHeight/(float)ScreenWidth);

        printf("%d by %d\n", ScreenWidth, ScreenHeight);
    }

    void Update(double ElapsedTime) override
    {
        // Clear Screen
        Fill({0, 0, ScreenWidth, ScreenHeight}, color{0, 0, 0, 255});

        // Set up rotation matrices
        mat4x4 RotZ;
        mat4x4 RotX;
        Theta += 1.0f * ElapsedTime;

        // Rotation Z
        // RotZ.m[0][0] = cosf(Theta);
        // RotZ.m[0][1] = sinf(Theta);
        // RotZ.m[1][0] = -sinf(Theta);
        // RotZ.m[1][1] = cosf(Theta);
        // RotZ.m[2][2] = 1;
        // RotZ.m[3][3] = 1;
        RotZ = MakeMatrix_RotZ(Theta);

        // Rotation X
        // RotX.m[0][0] = 1;
        // RotX.m[1][1] = cosf(Theta * 0.5f);
        // RotX.m[1][2] = sinf(Theta * 0.5f);
        // RotX.m[2][1] = -sinf(Theta * 0.5f);
        // RotX.m[2][2] = cosf(Theta * 0.5f);
        // RotX.m[3][3] = 1;
        RotX = MakeMatrix_RotX(Theta*0.5f);

        mat4x4 WorldTransform;
        // WorldTransform.m[0][0] = 1.0f;
        // WorldTransform.m[1][1] = 1.0f;
        // WorldTransform.m[2][2] = 1.0f;
        // WorldTransform.m[3][3] = 1.0f;

        mat4x4 Translation;
        // Translation.m[0][0] = 1.0f;
        // Translation.m[1][1] = 1.0f;
        // Translation.m[2][2] = 1.0f;
        // Translation.m[3][3] = 1.0f;
        // Translation.m[3][0] = 0;
        // Translation.m[3][1] = 0;
        // Translation.m[3][2] = 3;
        Translation = MakeMatrix_Translation(0, 0, 3.0f);

        WorldTransform = Matrix_MultiplyMatrix(RotZ, RotX);
        WorldTransform = Matrix_MultiplyMatrix(WorldTransform, Translation);
        
        // Draw Triangles
        for (auto Tri : CubeMesh.tris)
        {
            triangle ProjectedTri;
            triangle TranslatedTri;

            // Rotate and offset into the screen
            TranslatedTri.p[0] = Matrix_MultiplyVector(WorldTransform, Tri.p[0]);
            TranslatedTri.p[1] = Matrix_MultiplyVector(WorldTransform, Tri.p[1]);
            TranslatedTri.p[2] = Matrix_MultiplyVector(WorldTransform, Tri.p[2]);

            // Calculate normal and stuff
            v3f Normal;
            v3f Line1, Line2;

            Line1 = Vector_Sub(TranslatedTri.p[1], TranslatedTri.p[0]);
            Line2 = Vector_Sub(TranslatedTri.p[2], TranslatedTri.p[0]);

            Normal = Vector_CrossProduct(Line1, Line2);
            Normal = Vector_Normalize(Normal);

            if(Vector_DotProduct(Normal, Vector_Sub(TranslatedTri.p[0], Camera)) < 0)
            {
                v3f LightDirection = Vector_Normalize({0, 0, -1.0});
                Uint8 ColorValue = Vector_DotProduct(Normal, LightDirection) * 255;

                // Projection triangles from 3D --> 2D
                ProjectedTri.p[0] = Matrix_MultiplyVector(Projection, TranslatedTri.p[0]);
                ProjectedTri.p[1] = Matrix_MultiplyVector(Projection, TranslatedTri.p[1]);
                ProjectedTri.p[2] = Matrix_MultiplyVector(Projection, TranslatedTri.p[2]);

                // Scale into view
                ProjectedTri.p[0] = Vector_Div(ProjectedTri.p[0], ProjectedTri.p[0].w);
                ProjectedTri.p[1] = Vector_Div(ProjectedTri.p[1], ProjectedTri.p[1].w);
                ProjectedTri.p[2] = Vector_Div(ProjectedTri.p[2], ProjectedTri.p[2].w);

                // [-1, 1], [-1, 1] --> [0, 2], [0, 2]
                ProjectedTri.p[0].x += 1.0f;
                ProjectedTri.p[0].y += 1.0f;
                ProjectedTri.p[1].x += 1.0f;
                ProjectedTri.p[1].y += 1.0f;
                ProjectedTri.p[2].x += 1.0f;
                ProjectedTri.p[2].y += 1.0f;
                // [0, 2], [0, 2] --> [0, ScreenWidth], [0, ScreenHeight]
                ProjectedTri.p[0].x *= 0.5f * (float)ScreenWidth;
                ProjectedTri.p[0].y *= 0.5f * (float)ScreenHeight;
                ProjectedTri.p[1].x *= 0.5f * (float)ScreenWidth;
                ProjectedTri.p[1].y *= 0.5f * (float)ScreenHeight;
                ProjectedTri.p[2].x *= 0.5f * (float)ScreenWidth;
                ProjectedTri.p[2].y *= 0.5f * (float)ScreenHeight;
            
                // Rasterize triangle
                // FillTriangle(
                //         triangle2d{
                //             (int)ProjectedTri.p[0].x, (int)ProjectedTri.p[0].y,
                //             (int)ProjectedTri.p[1].x, (int)ProjectedTri.p[1].y,
                //             (int)ProjectedTri.p[2].x, (int)ProjectedTri.p[2].y},
                //         color{ColorValue, ColorValue, ColorValue, 255});
                FillTriangle(ExtractTriXY(ProjectedTri), color{ColorValue, ColorValue, ColorValue, 255});

                // DrawTriangle(
                //         triangle2d{
                //             (int)ProjectedTri.p[0].x, (int)ProjectedTri.p[0].y,
                //             (int)ProjectedTri.p[1].x, (int)ProjectedTri.p[1].y,
                //             (int)ProjectedTri.p[2].x, (int)ProjectedTri.p[2].y},
                //         color{0, 0, 0, 255});
                DrawTriangle(ExtractTriXY(ProjectedTri), color{0, 0, 0, 255});
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