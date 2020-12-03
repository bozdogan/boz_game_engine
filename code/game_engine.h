#ifndef GAME_ENGINE_H

#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif


struct v2d
{
    int x;
    int y;
};


struct rect2d
{
    int x;
    int y;
    int w;
    int h;
};

struct triangle2d
{
    v2d p[3];
};


struct game_engine
{
    SDL_Window *_sdl_window;
    SDL_Renderer *_sdl_renderer;

    int ScreenWidth;
    int ScreenHeight;
    bool Running;

    int
    Construct(int Width, int Height, char *Title)
    {
        if(SDL_Init(SDL_INIT_VIDEO))
        {
            // TODO(bora): Probably log this
            Running = 0;
            return 1;
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
        return 0;
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

    virtual void Update(float ElapsedTime)
    {
    }

    void Run()
    {
        Init();

        Uint32 tick_prev = 0;
        Uint32 tick = 0;

        SDL_Event event;
        while(Running)
        {
            tick_prev = tick;
            tick = SDL_GetTicks();
            float elapsed_time = (float)SDL_TICKS_PASSED(tick_prev, tick) / 1000.0;

            Update(elapsed_time);
            SDL_RenderPresent(_sdl_renderer);

            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }
    }

    void DrawPoint(v2d Point)
    {
        SDL_RenderDrawPoint(_sdl_renderer, Point.x, Point.y);
    }

    void DrawLine(v2d StartPos, v2d EndPos)
    {
        DrawLine(StartPos.x, StartPos.y, EndPos.x, EndPos.y);
    }

    void DrawLine(int StartX, int StartY, int EndX, int EndY)
    {
        SDL_RenderDrawLine(
                _sdl_renderer, 
                StartX, StartY,
                EndX, EndY);
    }

    void DrawTriangle(triangle2d Tri)
    {
        DrawLine(Tri.p[0], Tri.p[1]);
        DrawLine(Tri.p[1], Tri.p[2]);
        DrawLine(Tri.p[2], Tri.p[0]);
    }

    void FillRect(rect2d Rectangle)
    {
        SDL_RenderFillRect(_sdl_renderer, &SDL_Rect{
                    Rectangle.x,
                    Rectangle.y,
                    Rectangle.w,
                    Rectangle.h});
    }
};

#define GAME_ENGINE_H
#endif