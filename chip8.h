//
// Created by charl on 12/29/2019.
//

#include <stdint.h>

#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#define MEM_AMT       4096
#define PROGRAM_START 0x200
#define TOTAL_FONT_BYTES 80
#define STACK_SIZE 16
#define REG_AMT 16
#define KEY_AMT 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32


typedef uint8_t u8;
typedef uint16_t u16;


typedef struct chip8{

    /*
     *
    Memory Map:
            +---------------+= 0xFFF (4095) End of Chip-8 RAM
    |               |
    |               |
    |               |
    |               |
    |               |
    | 0x200 to 0xFFF|
    |     Chip-8    |
    | Program / Data|
    |     Space     |
    |               |
    |               |
    |               |
    +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
    |               |
    |               |
    |               |
    +---------------+= 0x200 (512) Start of most Chip-8 programs
    | 0x000 to 0x1FF|
    | Reserved for  |
    |  interpreter  |
    +---------------+= 0x000 (0) Start of Chip-8 RAM

     */



    u8 display[DISPLAY_WIDTH * DISPLAY_HEIGHT];

    u8 *ram;

    u8 stackPointer;
    u16 programCounter;

    //16-bit register used to store memory addresses. lowest 12 bits are used.
    u16 I;

    u8 drawFlag;

    //16 gen-purpose 8-bit registers. Called Vx where x is a hexadecimal digit.
    //VF is used as a flag, and shouldn't be accessible to any program
    u8 V[REG_AMT];

    //keys
    u8 *Keys;

    //stack is size 16
    u16 *stack;

    u8 delayTimer;
    u8 soundTimer;

    //the display is 64x32, monochrome

}chip8;

chip8* allocNewChip8();

void runChip8(chip8 *chip);

void resetMem(chip8 *chip);

void printChip8(chip8 *chip);

void debugDraw(chip8 *chip);


u8 loadProgram(chip8 *chip, char *fileName);

u8 readByte(chip8 *chip, u16 address);





#endif //CHIP8_CPU_H
