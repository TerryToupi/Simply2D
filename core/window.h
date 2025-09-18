#pragma once

#include <SDL3/SDL.h>

namespace core
{
    struct WindowSpecifications
    {
        const char* name = "Simply2D";
        int width = 640;
        int height = 480;
        bool resizable = false;
        bool fullScreen = false;
        bool vSync = false;
    };

    class Window
    {
    public:
        Window(const WindowSpecifications& specs = WindowSpecifications());
        ~Window();
        
        void create();
        void destroy();
        void update();
        
    private:
        WindowSpecifications m_specifications{};
        SDL_Window* m_handle{};
        SDL_Surface* m_surface{};
    };
}
