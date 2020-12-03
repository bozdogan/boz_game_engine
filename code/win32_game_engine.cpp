#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#define local_persist static

struct Demo :  game_engine
{
    void Init() override
    {
        srand(time(0));

        std::vector<v2d> points;
        for(int i = 0; i < 420; ++i)
            points.push_back(v2d{rand() % ScreenWidth,  rand() % ScreenHeight});

        SDL_SetRenderDrawColor(_sdl_renderer, 255, 0, 0, 255);
        for(auto it = points.begin(); it != points.end(); ++it)
            DrawPoint(*it);
    }

    void Update(double ElapsedTime) override
    {
        local_persist float timer;
        local_persist Uint8 shade;

        timer += ElapsedTime;
        if(timer >= .1)
        {
            timer -= .1;

            SDL_SetRenderDrawColor(_sdl_renderer, 255 - shade, 127, shade, 255);
            shade += 2;

            DrawTriangle(triangle2d{{
                        {450 + rand()%200, 100 + rand()%100},
                        {100 + rand()%100, 600 + rand()%100},
                        {800 + rand()%100, 600 + rand()%100}}});
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