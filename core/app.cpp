#include <app.h>
#include <SDL3/SDL.h>

namespace core
{
    Application::Application()
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        SDL_InitSubSystem(SDL_INIT_CAMERA);
        SDL_InitSubSystem(SDL_INIT_AUDIO);
        SDL_InitSubSystem(SDL_INIT_EVENTS);
        
    }
    
    Application::~Application()
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_QuitSubSystem(SDL_INIT_CAMERA);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        SDL_Quit();
    }
}
