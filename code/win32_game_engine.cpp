#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#define local_persist static

struct Demo :  game_engine
{
    void DrawPoints(std::vector<v2d> &points)
    {
        SDL_SetRenderDrawColor(_sdl_renderer, 255, 0, 0, 255);  
        for(auto it = points.begin(); it != points.end(); ++it)
        {
            FillRect({it->x, it->y, 1, 1});
        }
    }

    void Init() override
    {
        srand(time(0));

        std::vector<v2d> points;
        for(int i = 0; i < 420; ++i)
        {
            points.push_back(v2d{rand() % ScreenWidth,  rand() % ScreenHeight});
        }

        DrawPoints(points);
    }

    void Update(float ElapsedTime) override
    {
        local_persist float timer;
        local_persist Uint8 shade;
        local_persist int StartX = 200 + rand()%300;
        local_persist int StartY = 100 + rand()%200;

        timer += ElapsedTime;
        if(timer >= 1)
        {
            timer -= 1;

            SDL_SetRenderDrawColor(_sdl_renderer, 255 - shade, 0, 0 + shade, 255);
            shade += 16;

            DrawLine(StartX, StartY, rand()%ScreenWidth, rand()%ScreenHeight);
            DrawTriangle(triangle2d{{
                        {300 + rand()%100, 300 + rand()%100},
                        {100 + rand()%100, 500 + rand()%100},
                        {500 + rand()%100, 500 + rand()%100}}});
        }
    }
};


int main(int argc, char **argv)
{
    Demo game;
    if(!game.Construct(1000, 800, "ENGINE"))
        game.Run();
    
    return 0;
}