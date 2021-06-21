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

	FILE *fp;

	fp = fopen("chip8Log.txt", "w"); //open File for writing 	



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
	
	SDL_GL_CreateContext(window);

	SDL_GL_SwapWindow(window);	

	SDL_Renderer *renderer;
	SDL_Surface *surface;
    	
	
		
				

	SDL_Delay(2000);	

	SDL_Quit();	

    return 0;

}
