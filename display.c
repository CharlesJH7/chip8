#include "display.h"
#include "SDL2/SDL.h"

void createDisplay(Display *dsply){
	
	dsply->window = SDL_CreateWindow(
                                    "Chip-8", 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SDL_WINDOWPOS_CENTERED, 
                                    W * WINDOW_SCALE, H * WINDOW_SCALE,
                                    SDL_WINDOW_SHOWN
                                    );

	dsply->renderer = SDL_CreateRenderer(dsply->window, -1, SDL_RENDERER_ACCELERATED);
	
    dsply->texture = SDL_CreateTexture(dsply->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, W, H);

    

	SDL_SetRenderDrawColor(dsply->renderer, 0x00, 0x00,0x00, 0xFF);
    SDL_RenderPresent(dsply->renderer);

}

void updateDisplay(chip8 *chip, Display *dsply){
    
    SDL_SetRenderDrawColor(dsply->renderer, 0xFF, 0xFF, 0xFF, 1);
    
    for(int y = 0; y < DISPLAY_HEIGHT; y++){
        for(int x = 0; x < DISPLAY_WIDTH; x++){
            printf("%d", chip->display[x+(y*DISPLAY_WIDTH)]);
            if(chip->display[x + y * DISPLAY_WIDTH] == 1){
                SDL_Rect rect = {x * WINDOW_SCALE, y * WINDOW_SCALE, WINDOW_SCALE, WINDOW_SCALE};
                SDL_RenderFillRect(dsply->renderer, &rect);
            }
        }
    }

    //SDL_RenderCopy(dsply->renderer, dsply->texture, NULL, NULL);
    SDL_RenderPresent(dsply->renderer);

}

void destroyDisplay(Display *dsply){

    SDL_DestroyTexture(dsply->texture);
    SDL_DestroyRenderer(dsply->renderer);
    SDL_DestroyWindow(dsply->window);

    free(dsply);

    SDL_Quit();

}
