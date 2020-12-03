#include "game_engine.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#include <cstdio>

struct Demo :  game_engine
{
    Demo() : game_engine(1000, 800, "ENGINE")
    {
    }

    double penetration = 0; // ouch!

    void DrawPoints(std::vector<v2d> &points)
    {
        SDL_SetRenderDrawColor(_sdl_renderer, 255, 0, 0, 255);  
        for(auto it = points.begin(); it != points.end(); ++it)
        {
            const int ptwidth = 3;
            SDL_RenderFillRect(_sdl_renderer, &SDL_Rect{
                        (int) it->x - ptwidth/2,
                        (int) it->y - ptwidth/2,
                        ptwidth, ptwidth});
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
            printf("%d %d\n", ScreenWidth, ScreenHeight);

        DrawPoints(points);
    }

    void Update(double ElapsedTime) override
    {
        penetration += ElapsedTime;

        if(penetration > 2)
        {
            penetration -= 2;
            SDL_SetRenderDrawColor(_sdl_renderer, 0, 0, 0, 255);  
            SDL_RenderClear(_sdl_renderer);

            srand(time(0));

            std::vector<v2d> points;
            for(int i = 0; i < 420; ++i)
            {
                points.push_back(v2d{rand() % ScreenWidth,  rand() % ScreenHeight});
            }
            DrawPoints(points);
        }
            printf("%f\n", penetration);
    }
};


int main(int argc, char **argv)
{
    Demo *game = new Demo();
    game->Run();
    
    delete game;
    return 0;
}