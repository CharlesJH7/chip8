
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "display.h"
#include "chip8.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void initialize_window(SDL_Window **sdlWindow, SDL_Renderer **sdlRenderer){
		*sdlWindow =  SDL_CreateWindow("CHIP-8", //Window title
									SDL_WINDOWPOS_CENTERED, //x position							
									SDL_WINDOWPOS_CENTERED, //y position
									WINDOW_WIDTH, //width
									WINDOW_HEIGHT, //height
									SDL_WINDOW_SHOWN |						
									SDL_WINDOW_OPENGL |
									SDL_WINDOW_RESIZABLE |
									SDL_WINDOW_ALLOW_HIGHDPI
								  );

		if(sdlWindow == NULL){
			printf("Could not create window: %s\n", SDL_GetError());
			exit(101);
		}else{



	}




		

}

void sendGraphics(chip8 *chip8, u8 *buffer, SDL_Renderer *sdlRenderer){
	
}

void drawGraphics(){
	
}

void closeWindow(SDL_Window *sdlWindow, SDL_Renderer *sdlRenderer, SDL_Texture *sdlTexture){
	
} 


int main(int argc, char* args[]){
	
	
	

	return 0;
}






