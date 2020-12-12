#ifndef GAME_ENGINE_BASE_H

#include "game_engine_math.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

struct game_engine
{
    SDL_Window *_sdl_window;
    SDL_Renderer *_sdl_renderer;
    color _sdl_render_color;

    int ScreenWidth;
    int ScreenHeight;
    bool Running;
    bool AnyKeyDown;

    int Construct(int Width, int Height, char *Title)
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

    virtual void Update(double ElapsedTime)
    {
    }

    void Run()
    {
        Init();

        TimePoint tick_prev = Clock::now();
        TimePoint tick = Clock::now();

        SDL_Event event;
        while(Running)
        {
            tick_prev = tick;
            tick = Clock::now();
            std::chrono::duration<double> elapsed_time = tick - tick_prev;

            Update(elapsed_time.count());
            SDL_RenderPresent(_sdl_renderer);

            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_KEYDOWN)
                {
                    AnyKeyDown = true;
                }
                else if(event.type == SDL_KEYUP)
                {
                    AnyKeyDown = false;
                }
                else if (event.type == SDL_QUIT)
                {
                    Running = 0;
                }
            }
        }
    }

    void _SetRenderColor(color Color)
    {
        if(!ColorMatches(Color, _sdl_render_color))
        {
            _sdl_render_color = Color;

            SDL_SetRenderDrawColor(_sdl_renderer,
                    _sdl_render_color.r,
                    _sdl_render_color.g,
                    _sdl_render_color.b,
                    _sdl_render_color.a);
        }
    }

    // NOTE(bora): DrawPoint(int, int), DrawLine(int, int, int, int) and Fill(rect2d)
    // are what I call the "fundamental drawing routines". This functions handle
    // render color. All other drawing routines should use these rather than directly
    // calling SDL's functions unless there's an obvious advantage of doing so.
    void DrawPoint(v2i Point, color Color)
    {
        DrawPoint(Point.x, Point.y, Color);
    }

    void DrawPoint(int X, int Y, color Color)
    {
        _SetRenderColor(Color);
        SDL_RenderDrawPoint(_sdl_renderer, X, Y);
    }

    void DrawLine(v2i StartPos, v2i EndPos, color Color)
    {
        DrawLine(StartPos.x, StartPos.y, EndPos.x, EndPos.y, Color);
    }

    void DrawLine(int StartX, int StartY, int EndX, int EndY, color Color)
    {
        _SetRenderColor(Color);
        SDL_RenderDrawLine(
                _sdl_renderer, 
                StartX, StartY,
                EndX, EndY);
    }

    void Fill(rect2d Rectangle, color Color)
    {
        _SetRenderColor(Color);
        SDL_RenderFillRect(_sdl_renderer, &SDL_Rect{
                    Rectangle.x,
                    Rectangle.y,
                    Rectangle.w,
                    Rectangle.h});
    }

    void DrawTriangle(triangle2d Tri, color Color)
    {
        DrawLine(Tri.p[0], Tri.p[1], Color);
        DrawLine(Tri.p[1], Tri.p[2], Color);
        DrawLine(Tri.p[2], Tri.p[0], Color);
    }

    void FillTriangle(triangle2d Tri, color Color); // (see game_engine_draw.h)

    bool KeyDown(SDL_Keycode);
    bool KeyUp(SDL_Keycode);
};

#define GAME_ENGINE_BASE_H
#endif