
#include <SDL2/SDL.h>
#include "chip8.h"

#define W 64
#define H 32

typedef struct Display{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

}Display;


//initializes Display's window, renderer and texture
void createWindow(Display *display);

void updateWindow(u8 pixels[W * H], Display *display);

void destroyWindow(Display *dsply);
