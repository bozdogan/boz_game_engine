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

    v3f Camera;
    float Angle;

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

        Projection = MakeMatrix_Projection(
                0.1f, 1000.0f,
                90.0f,
                (float)ScreenHeight/(float)ScreenWidth);

        Camera = {0, 0, 0};
        Angle = 0;
        AnyKeyDown = false;

        printf("%d by %d\n", ScreenWidth, ScreenHeight);
    }

    void Update(double ElapsedTime) override
    {
        Fill({0, 0, ScreenWidth, ScreenHeight}, color{0, 0, 0, 255});
        if(!AnyKeyDown)
            Angle += 1.0f * ElapsedTime;

        mat4x4 RotZ = MakeMatrix_RotZ(Angle);
        mat4x4 RotX = MakeMatrix_RotX(Angle*0.5f);
        mat4x4 Translation = MakeMatrix_Translation(0, 0, 3.0f);

        mat4x4 WorldTransform;
        WorldTransform = Matrix_MultiplyMatrix(RotZ, RotX);
        WorldTransform = Matrix_MultiplyMatrix(WorldTransform, Translation);
        
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

                // Project triangles from 3D to 2D
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
            

                FillTriangle(ExtractTriXY(ProjectedTri), color{ColorValue, ColorValue, ColorValue, 255});

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