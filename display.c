#include "display.h"
#include "SDL2/SDL.h"

#define W 64
#define H 32


void createWindow(Display *dsply){
    dsply->window = SDL_CreateWindow(
                                    "Chip-8", 
                                    SDL_WINDOWPOS_CENTERED, 
                                    SDL_WINDOWPOS_CENTERED, 
                                    DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    SDL_WINDOW_RESIZABLE
                                    );

    dsply->renderer = SDL_CreateRenderer(dsply->window, -1, 0);

    dsply->texture = SDL_CreateTexture(dsply->renderer,
                                        SDL_PIXELFORMAT_ARGB8888, 
                                        SDL_TEXTUREACCESS_STREAMING, 
                                        DISPLAY_WIDTH, DISPLAY_HEIGHT
                                        );
}

void updateWindow(chip8 *chip, Display *dsply){
    u16 pixels[W*H];

    SDL_SetRenderDrawColor(dsply->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(dsply->renderer);

    if(chip->drawFlag == 1){
        
        for(u8 i = 0; i < (W * H); i++){
            u16 pixel = chip->display[i];
            
        }

    }
    

    SDL_UpdateTexture(dsply->texture, NULL, pixels, 64 * sizeof(u8));
    SDL_RenderClear(dsply->renderer);
    SDL_RenderCopy(dsply->renderer, dsply->texture, NULL, NULL);
    SDL_RenderPresent(dsply->renderer);

}

void destroyWindow(Display *dsply){
    SDL_DestroyWindow(dsply->window);
    SDL_DestroyRenderer(dsply->window);
    SDL_DestroyTexture(dsply->texture);

    free(dsply);

    SDL_Quit();
}