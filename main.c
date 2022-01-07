/* 
 *
 *
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 *
 */
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


int main(int argc, char *argv[]) {    

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);    

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

    Display *dsply = malloc(sizeof(dsply));
    createWindow(dsply);

    while(1) {
        printf("\n-----------------\n");
        printf("State: %i", i);
        printf("\n-----------------\n");

        printChip8(chip);
        runChip8(chip);
        updateWindow(chip->display, dsply);
		i++;

		SDL_Event e;
	
		if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				break;
			}

		}					
    }

    debugDraw(chip);
    
    //destroyWindow(dsply);

    //free(chip);

    return 0;

}
