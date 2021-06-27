#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by charl on 12/29/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"
#include "instructions.h"



chip8* allocNewChip8(){

    chip8* chip = malloc(sizeof(chip8));

    //Should be stored in chip8 memory, 0x000 to 0x1FF
    unsigned char chip8FontSet[TOTAL_FONT_BYTES] = {
        //5 by 16 == 80
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80, //F
    };


	for(int x = 0; x < DISPLAY_WIDTH; x++){
		for(int y = 0; y < DISPLAY_HEIGHT; y++){
			chip->Display[x * y] = 0;
		}
	}   


    //program counter starts at 0x200
    chip->programCounter = PROGRAM_START;

    memset(chip->V, 0, REG_AMT);    //0 out registers
    chip->ram = malloc(MEM_AMT);    //allocate memory for ram

    //copy font set bytes into ram
    memcpy(chip->ram, chip8FontSet, TOTAL_FONT_BYTES);

    chip->stackPointer = 0x52;
    chip->I = 0;

    chip->drawFlag = 0;

    chip->delayTimer = 0;
    chip->soundTimer = 0;

    chip->Keys = calloc(sizeof(chip->Keys), KEY_AMT);
    chip->stack = calloc(sizeof(chip->stack), STACK_SIZE);

    return chip;
}


void printChip8(chip8 *chip){

    printf("\nChip8 State:\n");

    printf("\tProgram Counter: %04X\n", chip->programCounter);
    printf("\tI: %03X\n\n", chip->I);
    printf("\t*I: %03X\n", *(chip->ram + chip->I));
    printf("\t*I + 1: %03X\n", *(chip->ram + (chip->I + 1)));
    printf("\t*I + 2: %03X\n", *(chip->ram + (chip->I + 2)));
    printf("\t*I + 3: %03X\n", *(chip->ram + (chip->I + 3)));
    printf("\t*I + 4: %03X\n\n", *(chip->ram + (chip->I + 4)));

    printf("\tV%X: %02X\tV%X: %02X\tV%X: %02X\tV%X: %02X\t\n",
           0, chip->V[0], 1, chip->V[1], 2, chip->V[2], 3, chip->V[3]);
    printf("\tV%X: %02X\tV%X: %02X\tV%X: %02X\tV%X: %02X\t\n",
           4, chip->V[4], 5, chip->V[5], 6, chip->V[6], 7, chip->V[7]);
    printf("\tV%X: %02X\tV%X: %02X\tV%X: %02X\tV%X: %02X\t\n",
           8, chip->V[8], 9, chip->V[9], 10, chip->V[10], 11, chip->V[11]);
    printf("\tV%X: %02X\tV%X: %02X\tV%X: %02X\tV%X: %02X\t\n",
           12, chip->V[12], 13, chip->V[13], 14, chip->V[14], 15, chip->V[15]);

    printf("\n\tram: %04X", *chip->ram);
    printf("\tStack: %04X\n", *chip->stack);
    printf("\tsound: %02X", chip->soundTimer);
    printf("\tdelay: %02X\n", chip->delayTimer);

    printf("\n\tStack Pointer: %01X\n", chip->stackPointer);

    printf(":Chip8 End State\n\n");

}

//Reset ram
void resetMem(chip8 *chip){
    printf("Clearing ram...");
    memset(chip->ram + 0x200, 0, MEM_AMT - PROGRAM_START);
}

//load program into chip8 ram space
u8 loadProgram(chip8 *chip, char *fileName){

    FILE* f = fopen(fileName, "r");

    //if file doesn't exist, etc.
    if(f == NULL){
        fprintf(stderr, "File, %s, could not be found", fileName);
        return 1;
    }else{

        //go to end of file
        fseek(f, 0, SEEK_END);
        //get fileSize
        u16 fileSize = ftell(f);

        //printf("%d\n", fileSize);

        //reset cursor to beginning of file
        rewind(f);

        //if the filesize is greater than the memory the chip8 has allocated for program space
        if(fileSize > MEM_AMT - PROGRAM_START){
            fprintf(stderr, "File is too large: %i bytes", fileSize);
            return 1;
        }

        //read the file into the RAM's program space
	fread(chip->ram + PROGRAM_START, 1, fileSize, f);
    
    }

    fclose(f);

    return 0;
}

u8 readByte(chip8 *chip, u16 address){
	return *(chip->ram + address);
}


void runChip8(chip8 *chip){

    int debug = 1;

    u8 high = readByte(chip, chip->programCounter); // High byte
    u8 low = readByte(chip, chip->programCounter + 1); //Low byte

    u16 Opcode = (high << 8 ) | low; //put high in the high byte, low in the low

    //nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    //n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    //x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    //y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    //kk or byte - An 8-bit value, the lowest 8 bits of the instruction

    u8 x = (Opcode & 0x0F00) >> 8;
    u8 y = (Opcode & 0x00F0) >> 4;
    u8 n = Opcode & 0x000F;
    u8 kk = Opcode & 0x00FF;
    u16 nnn = Opcode & 0x0FFF;

    if(debug){

        printf("\nOpcode: 0x%04X\n------------------\n", Opcode);
        printf("x: 0x%1X\n", x);
        printf("y: 0x%1X\n", y);
        printf("n: 0x%1X\n", n);
        printf("kk: 0x%02X\n", kk);
        printf("nnn: 0x%03X\n\n", nnn);

    }

    switch(Opcode & 0xF000){

        case 0x0000:
            switch(kk){
                case 0xE0: //clear
                    printf("%04X CLS\n", kk);
                    CLS(chip);
                    break;
                case 0xEE: //return
                    printf("%04X RET\n", kk);
                    RET(chip);
                    break;
                default:
                    UNKWN_Opcode(Opcode);
                    break;
            }

        break;


        case 0x1000: //jump to address
            if(debug)
                printf("%04X Jumping to address: %04X \n", Opcode, nnn);
            JPaddr(chip, nnn);
            break;
        case 0x2000: //call address nnn
            if(debug)
                printf("%04X Call address: %03X \n", Opcode, nnn);
            CALLaddr(chip, nnn);
            break;
        case 0x3000:
            if(debug)
                printf("%04X Skip next instruction "
                   "if V[%01X] == %02X\n", Opcode, x, kk);
            SEVXbyte(chip, x, kk);
            break;
        case 0x4000:
            if(debug)
                printf("%04X Skip next instruction "
                   "if V[%01X] != %02X\n", Opcode, x, kk);
            SNEVXbyte(chip, x, kk);
            break;
        case 0x5000:
            if(debug)
                printf("%04X Skip next instruction "
                   "if V[%01X] == V[%01X]\n", Opcode, x, y);
            SEVXVY(chip, x, y);
            break;
        case 0x6000:
            if(debug)
                printf("%04X Set V[%01X] to %02X\n", Opcode, x, kk);
            LDVXbyte(chip, x, kk);
            break;
        case 0x7000:
            if(debug)
                printf("%04X Set V[%01X] to "
               "V[%01X] + %02X\n", Opcode, x, x, kk);
            ADDVXbyte(chip, x, kk);
            break;
        case 0x8000:
            switch(n){
                case 0x0:
                    if(debug)
                        printf("%04X Load V[%1X] into V[%1X]\n", Opcode, y, x);
                    LDVXVY(chip, x, y);
                break;
                case 0x1:
                    if(debug)
                        printf("%04X Or V[%1X] and V[%1X], "
                               "store into V[%1X]\n", Opcode, x, y, x);
                    ORVXVY(chip, x, y);
                break;
                case 0x2:
                    if(debug)
                        printf("%04X AND V[%1X] and V[%1X], "
                               "store into V[%1X]\n", Opcode, x, y, x);
                    ANDVXVY(chip, x, y);
                    break;
                case 0x3:
                    if(debug)
                        printf("%04X XOR V[%1X] and V[%1X], "
                               "store into V[%1X]\n", Opcode, x, y, x);
                    XORVXVY(chip, x, y);
                    break;
                case 0x4:
                    if(debug)
                        printf("%04X ADD V[%1X] and V[%1X], "
                               "store into V[%1X]\n", Opcode, x, y, x);
                    ADDVXVY(chip, x, y);
                    break;
                case 0x5:
                    if(debug)
                        printf("%04X V[%1X] - V[%1X], "
                               "store into V[%1X]\n", Opcode, x, y, x);
                    SUBVXVY(chip, x, y);
                    break;
                case 0x6:
                    if(debug)
                        printf("%04X RSHIFT V[%1X]\n", Opcode, x);
                    SHRVX(chip, x);
                    break;
                case 0x7:
                    if(debug)
                        printf("%04X V[%1X] - V[%1X], "
                               "store into V[%1X]\n", Opcode, y, x, x);
                    SUBNVXVY(chip, x, y);
                    break;
                case 0xE:
                    if(debug)
                        printf("%04X LSHIFT V[%1X]\n", Opcode, x);
                    SHLVX(chip, x);
                    break;
                default:
                    UNKWN_Opcode(Opcode);
                    break;
            }

            break;

        case 0x9000:
            if(debug)
                printf("%04X Skip instruction if "
                       "%01X != %01X\n", Opcode, x, y);
            SNEVXVY(chip, x, y);
            break;

        case 0xA000:
            if(debug)
                printf("%04X Load addr: %03X into I\n", Opcode, nnn);
            LDIaddr(chip, nnn);
            break;

        case 0xB000:
            if(debug)
                printf("%04X Jump to address %03X + V[0]: %01X\n", Opcode, nnn, chip->V[0]);
            JMPnnnV0(chip, nnn);
            break;

        case 0xC000:
            if(debug)
                printf("%04X Set V[%01X] to a random byte ANDed "
                       "with the value %02X\n", Opcode, x, kk);
            RNDVXbyte(chip, x, kk);
            break;

        case 0xD000:
            if(debug)
                printf("%04X Draw %01x-byte sprite in memory location %04X at (%i, %i) "
                       "Set V[F] = collision\n", Opcode, n, chip->I, chip->V[x], chip->V[y]);
            DRWsprite(chip, chip->V[x], chip->V[y], n);
            break;

        case 0xE000:
            switch(kk){
                case 0x9E:
                    if(debug)
                        printf("%04X Skip next instruction if the key with the value "
                               "in V[%01X] is pressed\n", Opcode, x);
                    SKPVX(chip, x);
                    break;
                case 0xA1:
                    if(debug)
                        printf("%04X Skip next instruction if the key with the value "
                               "in V[%01X] is NOT pressed\n", Opcode, x);
                    SKNPVX(chip, x);
                    break;
                default:
                    UNKWN_Opcode(Opcode);
                    break;
            }

        case 0xF000:
            switch(kk){
                case 0x07:
                    if(debug)
                        printf("%04X Set V[%01X] to delayTimer\n", Opcode, x);
                    LDVXDT(chip, x);
                    break;
                case 0x0A:
                    if(debug)
                        printf("%04X Store value of key into V[%01X]\n", Opcode, x);
                    LDVXK(chip, x);
                    break;
                case 0x15:
                    if(debug)
                        printf("%04X Set delayTimer to V[%01X]\n", Opcode, x);
                    LDDTVX(chip, x);
                    break;
                case 0x18:
                    if(debug)
                        printf("%04X Set soundTimer to V[%01X]\n", Opcode, x);
                    LDSTVX(chip, x);
                    break;

                case 0x1E:
                    if(debug)
                        printf("%04X Set I equal to %02X + V[%01X]\n", Opcode, chip->I, x);
                    ADDIVX(chip, x);
                    break;

                case 0x29:
                    if(debug)
                        printf("%04X Set I equal to location of sprite for digit "
                           "V[%01X]\n", Opcode, x);
                    LDFVX(chip, x);
                    break;

                case 0x33:
                    if(debug)
                        printf("%04X Store BCD representation of V[%01X] "
                               "into memory locations %04X, "
                               "%04X, %04X\n", Opcode, x, chip->I, chip->I + 1, chip->I + 2);
                    LDBVX(chip, x);
                    break;

                case 0x55:
                    if(debug)
                        printf("%04X Store registers V[0] through V[%01X] "
                               "into memory location %04X through %04X\n",
                               Opcode, x, chip->I, (chip->I + x));


                    LDIVX(chip, x);
                    break;

                case 0x65:
                    if(debug)
                        printf("%04X Read into registers V[0] through V[%01X] "
                               "from memory location %04X through %04X\n",
                               Opcode, x, chip->I, (chip->I + x));
                    LDVXI(chip, x);
                    break;

                default:
                    UNKWN_Opcode(Opcode);
                    break;
            }
            break;

        default:
            UNKWN_Opcode(Opcode);
            break;
    }

	chip->programCounter += 2;

}

void debugDraw(chip8 *chip){
	for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++){
        if(chip->Display[i] == 1){
            printf("█");
        }else{
            printf("░");
        }

        if(i % 64 == 0){
            printf("\n");
        }
    }     
}


#pragma clang diagnostic pop
