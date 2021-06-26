
#include <SDL2/SDL.h>

typedef struct SDL_Display{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface surface;

}display;

initializeDisplay();

updateDisplay();
