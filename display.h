
#include <SDL2/SDL.h>
#include "chip8.h"

typedef struct Display{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *texture;

}Display;


//initializes Display's window, renderer and texture
void createWindow(Display *display);

void updateWindow(chip8 *chip, Display *display);

void destroyWindow(Display *dsply);
