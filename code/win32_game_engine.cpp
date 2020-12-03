#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif
#include <cstdlib>
#include <ctime>
#include <vector>

struct v2d
{
    int x;
    int y;
};


void DrawPoints(SDL_Renderer *Renderer, std::vector<v2d> &points)
{
    SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);  
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        const int ptwidth = 3;
        SDL_RenderFillRect(Renderer, &SDL_Rect{
                    (int) it->x - ptwidth/2,
                    (int) it->y - ptwidth/2,
                    ptwidth, ptwidth});
    }
}


int main(int argc, char **argv)
{
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    int Width = 1000;
    int Height = 800;
    int Running = 1;

    srand(time(0));

    SDL_Window *Window = SDL_CreateWindow(
            "ENGINE",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            Width, Height, 0);
    SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

    std::vector<v2d> points;
    for(int i = 0; i < 420; ++i)
    {
        points.push_back(v2d{rand() % Width - 1,  rand() % Height});
    }

    DrawPoints(Renderer, points);

    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(Renderer, &SDL_Rect{-1, -1, 3, 3});

    SDL_RenderPresent(Renderer);

    SDL_Event event;
    while(Running)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            Running = 0;
        }
    }

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}