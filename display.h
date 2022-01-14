
#include <SDL2/SDL.h>
#include "chip8.h"

#define W 64
#define H 32

#define WINDOW_SCALE 15

typedef struct Display{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

}Display;


//initializes Display's window, renderer and texture
void createDisplay(Display *display);

//updates 
void updateDisplay(chip8 *chip, Display *display);

//free Display's window, renderer, texture
void destroyDisplay(Display *dsply);






