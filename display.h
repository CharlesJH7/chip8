
#include <SDL2/SDL.h>
#include "chip8.h"

typedef struct Display{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *texture;

}Display;


//initializes Display's window, renderer and texture
int initializeDisplay(Display *display);

//Returns the SDL Surface of the chip8's current display frame
SDL_Surface *getDisplay(chip8 *chip);

void updateDisplay(Display *display);
