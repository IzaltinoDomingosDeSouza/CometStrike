#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main()
{

    //TODO Create a Simple Game Application class
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window * window = SDL_CreateWindow("Comet Strike", 800, 600, 0);
    if (!window)
    {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    //Create Renderer class
    SDL_Renderer * renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while(running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
