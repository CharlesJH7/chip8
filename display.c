#include "display.h"
#include "SDL2/SDL.h"

void createWindow(Display *dsply){

    SDL_Init(SDL_INIT_VIDEO);

    dsply->window = SDL_CreateWindow(
                                    "Chip-8", 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SDL_WINDOWPOS_CENTERED, 
                                    W * WINDOW_SCALE, H * WINDOW_SCALE,
                                    SDL_WINDOW_SHOWN
                                    );

    dsply->renderer = SDL_CreateRenderer(dsply->window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_SetRenderDrawColor(dsply->renderer, 0xFF,0xFF,0xFF,0xFF);

    dsply->texture = SDL_CreateTexture(dsply->renderer, 
									   SDL_PIXELFORMAT_ARGB8888, 
										SDL_TEXTUREACCESS_STATIC, 
									    W, H);

   }

void updateWindow(u8 pixels[W * H], Display *dsply){
    
    SDL_RenderClear(dsply->renderer);
    SDL_UpdateTexture(dsply->texture, NULL, pixels, W * sizeof(u8));
    
    SDL_RenderCopy(dsply->renderer, dsply->texture, NULL, NULL);
    SDL_RenderPresent(dsply->renderer);
}

void destroyWindow(Display *dsply){

    SDL_DestroyTexture(dsply->texture);
    SDL_DestroyRenderer(dsply->renderer);
    SDL_DestroyWindow(dsply->window);

    SDL_Quit();
}
