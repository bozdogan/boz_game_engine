#ifndef GAME_ENGINE_H
#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif
#include <ctime>

struct v2d
{
    int x;
    int y;
};


struct game_engine
{
    SDL_Window *_sdl_window;
    SDL_Renderer *_sdl_renderer;

    int ScreenWidth;
    int ScreenHeight;
    bool Running;

    game_engine(int Width, int Height, char *Title)
    {
        if(SDL_Init(SDL_INIT_VIDEO))
        {
            // TODO(bora): Log this!
            Running = 0;
            return;
        }

        ScreenWidth = Width;
        ScreenHeight = Height;

        _sdl_window = SDL_CreateWindow(
                Title,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                Width, Height, 0);
        _sdl_renderer = SDL_CreateRenderer(_sdl_window, -1, 0);

        Running = 1;
    }

    ~game_engine()
    {
        SDL_DestroyRenderer(_sdl_renderer);
        SDL_DestroyWindow(_sdl_window);
        SDL_Quit();
    }

    // NOTE(bora): Init and Update are user functions. Intended way to use this library
    // is subclassing it so user can access the data provided by class, such as elapsed
    // time between frames. I don't like the idea of using virtual functions but this
    // is what works, for now.
    virtual void Init()
    {
    }

    virtual void Update(double ElapsedTime)
    {
    }

    void Run()
    {
        Init();

        clock_t tick_prev;
        clock_t tick = clock();

        SDL_Event event;
        while(Running)
        {
            tick_prev = tick;
            tick = clock();
            double elapsed_time = (double) (tick - tick_prev)/CLOCKS_PER_SEC;
            
            Update(elapsed_time);
            SDL_RenderPresent(_sdl_renderer);

            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }
    }
};


#define GAME_ENGINE_H
#endif