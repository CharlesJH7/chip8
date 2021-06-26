/* 
 *
 *
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

//http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php


int main(int argc, char *argv[]) {    

    chip8* chip = allocNewChip8();
 
    printf("\n");

    if(argc > 1 && argc < 3){
        for(int i = 0; i < argc; i++){
            printf("argv[%d] = %s\n", i, argv[i]);
        }

    }else{
        printf("\nUsage: %s <game name>\n", argv[0]);
        exit(1);
    }

    loadProgram(chip, argv[1]);

    int i = 0;


    while(1) {
        printf("\n-----------------\n");
        printf("State: %i", i);
        printf("\n-----------------\n");

        printChip8(chip);
        runChip8(chip);
        debugDraw(chip);
		i++;
			

        if(i == 200){
            break;
        }

    }
	
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Chip-8", 0,0,640,480, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_UpdateTexture(texture, NULL, chip->Display, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
	
	
		
	

	SDL_Delay(2000);	

	SDL_Quit();	

    return 0;

}
