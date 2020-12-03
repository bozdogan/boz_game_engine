#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#define local_persist static

struct Demo :  game_engine
{
    Demo() : game_engine(1000, 800, "ENGINE")
    {
    }

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
        timer += ElapsedTime;
        if(timer >= .5)
        {
            timer -= 1;

            SDL_SetRenderDrawColor(_sdl_renderer, 255 - shade, 0, 0 + shade, 255);
            shade += 16;

            DrawLine({400, 250}, {rand()%ScreenWidth, rand()%ScreenHeight});
        }
    }
};


int main(int argc, char **argv)
{
    Demo *game = new Demo();
    game->Run();
    
    delete game;
    return 0;
}