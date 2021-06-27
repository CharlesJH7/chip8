#include "display.h"
#include "SDL2/SDL.h"

//Initializes SDL2 and creates a window and renderer to display chip8 graphics
int initializeDisplay(Display *display){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    
    display->window = SDL_CreateWindow("Chip-8", 0,0,640,480, SDL_WINDOW_OPENGL);
    display->renderer = SDL_CreateRenderer(display->window, -1, 0);
    display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    return 0;
}
	
    

void updateDisplay(Display *display){
    int length = 64 * 32;
    u8 pixels[length];

    for(int i = 0; i < length; i++){
        u8 pixel = pixels[length];
        

    }




}

void closeDisplay(){



}