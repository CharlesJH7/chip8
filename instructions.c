#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc32-c"
#pragma ide diagnostic ignored "cert-msc30-c"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by charl on 1/5/2020.
//

#include "chip8.h"
#include "instructions.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
//Used as Reference

/*
 * nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
 * n or nibble - A 4-bit value, the lowest 4 bits of the instruction
 * x - A 4-bit value, the lower 4 bits of the high byte of the instruction
 * y - A 4-bit value, the upper 4 bits of the low byte of the instruction
 * kk or byte - An 8-bit value, the lowest 8 bits of the instruction
 *
 */

//0nnn - Sys Address
//Jump to a machine code routine at nnn
/*
     *This instruction is only used on the old computers
     * on which Chip-8 was originally implemented.
     * It is ignored by modern interpreters.
*/

void SysAddr(chip8 *chip, u16 nnn){
    

}

//00E0 - CLS
//Clear the Display

void CLS(chip8 *chip){

    memset(chip->Display, 0, sizeof(chip->Display));

}



//00EE - RET
//Return from a subroutine
//The interpreter sets the program counter to the address at the top of the stack,
//then subtracts 1 from the stack pointer.

void RET(chip8 *chip){
    --chip->stackPointer;
    chip->programCounter = chip->stack[chip->stackPointer];
}

//1nnn - JP addr
//Call addr
//The interpreter sets the program counter to nnn.
//WORKS
void JPaddr(chip8 *chip, u16 nnn){
    chip->programCounter = nnn;
}

//2nnn - CALL addr
//Call subroutine at nnn
//The interpreter increments the stack pointer,
// then puts the current PC on the top of the stack.
// The PC is then set to nnn.

void CALLaddr(chip8 *chip, u16 nnn){
    chip->stack[chip->stackPointer] = chip->programCounter;
    ++chip->stackPointer;
    chip->programCounter = nnn;
}

//3xkk - SE Vx, byte
//Skip next instruction if Vx == kk
//The interpreter compares register Vx to kk, and
//if they are equal, increments the program counter by 2.
//WORKS
void SEVXbyte(chip8 *chip, u8 x, u8 kk){
    if(chip->V[x] == kk){
        chip->programCounter += 2;
    }
}

//4xkk - SNE Vx, byte
//Skip next instruction if Vx != kk
//The interpreter compares register Vx to kk,
//and if they are not equal, increments the program counter by 2.
//WORKS
void SNEVXbyte(chip8 *chip, u8 x, u8 kk){
    if(chip->V[x] != kk){
        chip->programCounter += 2;
    }
}

//5xy0 - SE Vx, Vy
//Skip next instruction if Vx == Vy
//The interpreter compares register Vx to register Vy,
//and if they are equal, increments the program counter by 2.
//WORKS
void SEVXVY(chip8 *chip, u8 x, u8 y){
    if(chip->V[x] == chip->V[y]){
        chip->programCounter += 2;
    }
}

//6xkk - LD Vx, byte
//Set Vx = kk
//The interpreter puts the value kk into register Vx.
//WORKS
void LDVXbyte(chip8 *chip, u8 x, u8 kk){
    chip->V[x] = kk;
}

//7xkk - ADD Vx, byte
//Set Vx = Vx + kk
//Adds the value kk to the value of register Vx, then stores the result in Vx.
//WORKS
void ADDVXbyte(chip8 *chip, u8 x, u8 kk){
    chip->V[x] = chip->V[x] + kk;
}

//8xy0 - LD Vx, Vy
//Set Vx = Vy
//Stores the value of register Vy in register Vx.

void LDVXVY(chip8 *chip, u8 x, u8 y){
    chip->V[x] = chip->V[y];
}

//8xy1 - OR Vx, Vy
//Set Vx = Vx OR Vy
//Performs a bitwise OR on the values of Vx and Vy,
// then stores the result in Vx.

void ORVXVY(chip8 *chip, u8 x, u8 y){
    chip->V[x] = chip->V[x] | chip->V[y];
}

//8xy2 - AND Vx, Vy
//Set Vx = Vx AND Vy
//Performs a bitwise AND on the values of Vx and Vy,
// then stores the result in Vx.
void ANDVXVY(chip8 *chip, u8 x, u8 y){
    chip->V[x] = chip->V[x] & chip->V[y];
}


//8xy3 - XOR Vx, Vy
//Set Vx = Vx XOR Vy
//Performs a bitwise exclusive OR on the values of Vx and Vy,
// then stores the result in Vx.

void XORVXVY(chip8 *chip, u8 x, u8 y){
    chip->V[x] = chip->V[x] ^ chip->V[y];
}

//8xy4 - ADD Vx, Vy
//Set Vx = Vx + Vy, set VF = carry
//The values of Vx and Vy are added together.
// If the result is greater than 8 bits (i.e., > 255,)
// VF is set to 1, otherwise 0. Only the lowest 8 bits
// of the result are kept, and stored in Vx.

void ADDVXVY(chip8 *chip, u8 x, u8 y){

    u16 upperBits = 0xFF00; //mask
    u16 result = chip->V[x] + chip->V[y]; //u16 result to hold addition
    chip->V[x] = result; //set u8 equal to u16

    if(upperBits & result){ //if the value > 255
        chip->V[0xF] = 1; //set flag 1
    }else{
        chip->V[0xF] = 0; //otherwise flag is 0
    }
}

//8xy5 - SUB Vx, Vy
//Set Vx = Vx - Vy, set VF = NOT borrow
//If Vx > Vy, then VF is set to 1, otherwise 0.
// Then Vy is subtracted from Vx, and the results stored in Vx.
//WORKS
void SUBVXVY(chip8 *chip, u8 x, u8 y){
    if(chip->V[x] > chip->V[y]){
        chip->V[0xF] = 1;
    }else{
        chip->V[0xF] = 0;
    }
    chip->V[x] = chip->V[x] - chip->V[y]; //Vx = Vx - Vy

}

//8xy6 - SHR Vx {, Vy}
//Set Vx = Vx SHR 1
//If the least-significant bit of Vx is 1,
//then VF is set to 1, otherwise 0. Then Vx is divided by 2.
//WORKS
void SHRVX(chip8 *chip, u8 x){
    if(chip->V[x] & 0x1){
        chip->V[0xF] = 1;
    }else{
        chip->V[0xF] = 0;
    }
    chip->V[x] = chip->V[x] >> 1;

}

//8xy7 - SUBN Vx, Vy
//Set Vx = Vy - Vx, set VF = NOT borrow
//If Vy > Vx, then VF is set to 1, otherwise 0.
// Then Vx is subtracted from Vy, and the results stored in Vx.
//
void SUBNVXVY(chip8 *chip, u8 x, u8 y){

    if(chip->V[y] > chip->V[x]){
        chip->V[0xF] = 1;
    }else{
        chip->V[0xF] = 0;
    }

    chip->V[x] = chip->V[y] - chip->V[x]; // Vx = Vy - Vx

}

//8xyE - SHL Vx {, Vy}
//Set Vx = Vx SHL 1
//If the most-significant bit of Vx is 1,
// then VF is set to 1, otherwise to 0.
// Then Vx is multiplied by 2.
//WORKS
void SHLVX(chip8 *chip, u8 x){
    if(0x80 & chip->V[x]){
        chip->V[0xF] = 1;
    }else{
        chip->V[0xF] = 0;
    }
    chip->V[x] = chip->V[x] << 1;

}

//9xy0 - SNE Vx, Vy
//Skip next instruction if Vx != Vy
//The values of Vx and Vy are compared,
// and if they are not equal, the program
// counter is increased by 2.

void SNEVXVY(chip8 *chip, u8 x, u8 y){
    if(chip->V[x] != chip->V[y]){
        chip->programCounter += 2;
    }
}

//Annn - LD I, addr
//Set I = nnn
//The value of register I is set to nnn.
//WORKS
void LDIaddr(chip8 *chip, u16 nnn){
    chip->I = nnn;
}

//Bnnn - JP V0, addr
//Jump to location nnn + V0
//The program counter is set to nnn plus the value of V0.

void JMPnnnV0(chip8 *chip, u16 nnn){
    chip->programCounter = nnn + chip->V[0];
}

//Cxkk - RND Vx, byte
//Set Vx = random byte AND kk
//The interpreter generates a random number from 0 to 255,
// which is then ANDed with the value kk. The results are
// stored in Vx. See instruction 8xy2 for more information on AND.

void RNDVXbyte(chip8 *chip, u8 x, u8 kk){
    int randomNum = rand() % 256;

    chip->V[x] = randomNum & kk;
}

//Pseudocode for DRAW instruction
//http://www.emulator101.com/chip-8-sprites.html

//Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
// The interpreter reads n bytes from memory, starting at the address stored in I.
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
// VF is set to 1, otherwise it is set to 0. If the sprite is positioned
// so part of it is outside the coordinates of the display,
// it wraps around to the opposite side of the screen.

//(0,0)	    (63,0)
//(0,31)	(63,31)

void DRWsprite(chip8 *chip, u8 x, u8 y, u8 n){

	u8 xPos = chip->V[x];
	u8 yPos = chip->V[y];
	u8 pixel;

	printf("xPos: %X yPos: %X", xPos, yPos);
	
	chip->V[0xF] = 0;

	for(u8 yCoord = 0; yCoord < n; yCoord++){
		pixel = chip->ram[chip->I + yCoord];

		for(u8 xCoord = 0; xCoord < 8; xCoord++){ 
			if((pixel & (0x80 >> xCoord)) != 0){
				if(chip->Display[yPos + yCoord][xPos + xCoord] == 1){
					chip->V[0xF] = 1;
				} 
				
				chip->Display[yPos + yCoord][xPos + xCoord] ^= 1;
			}	
		}
	}

	chip->drawFlag = 1;
	
}

//Ex9E - SKP Vx
//Skip next instruction if key with the value of Vx is pressed
//Checks the keyboard, and if the key corresponding
// to the value of Vx is currently in the down position,
// PC is increased by 2.

void SKPVX(chip8 *chip, u8 x){
    if(chip->Keys[chip->V[x]]){
        chip->programCounter += 2;
    }
}

//ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed
//Checks the keyboard, and if the key
// corresponding to the value of Vx is currently in the up position,
// PC is increased by 2.

void SKNPVX(chip8 *chip, u8 x){
    if(!chip->Keys[chip->V[x]]){
        chip->programCounter += 2;
    }
}

//Fx07 - LD Vx, DT
//Set Vx = delay timer value
//The value of DT is placed into Vx.

void LDVXDT(chip8 *chip, u8 x){
    chip->V[x] = chip->delayTimer;
}

//Fx0A - LD Vx, K
//Wait for a key press, store the value of the key in Vx
//All execution stops until a key is pressed,
//then the value of that key is stored in Vx.

void LDVXK(chip8 *chip, u8 x){
    //stop execution
    while(1){
        for(int i = 0; i < KEY_AMT; i++){ //check every key
            if(chip->Keys[i]){ //if key is pressed
                chip->V[x] = i; //have V[x] store value of key
                break;//leave loop
            }
        }
        break; //got key value, leave while loop
    }
}

//Fx15 - LD DT, Vx
//Set delay Timer = Vx
//DT is set equal to the value of Vx.

void LDDTVX(chip8 *chip, u8 x){
    chip->delayTimer = chip->V[x];
}

//Fx18 - LD ST, Vx
//Set sound timer = Vx
//ST is set equal to the value of Vx.

void LDSTVX(chip8 *chip, u8 x){
    chip->soundTimer = chip->V[x];
}

//Fx1E - ADD I, Vx
//Set I = I + Vx
//The values of I and Vx are added, and the results are stored in I.
//WORKS
void ADDIVX(chip8 *chip, u8 x){
    chip->I = chip->I + chip->V[x];
}

//Fx29 - LD F, Vx
//Set I = location of sprite for digit Vx
//The value of I is set to the location for the hexadecimal
//sprite corresponding to the value of Vx.
void LDFVX(chip8 *chip, u8 x){
    chip->I = BYTES_PER_CHAR_SPRITE * chip->V[x];
}

//Fx33 - LD B, Vx
//Store BCD representation of Vx in memory locations I, I+1, and I+2
//The interpreter takes the decimal value of Vx, and places the hundreds digit
//in memory at location I, the tens digit at location I+1,
//and the ones digit at location I+2.
//WORKS
void LDBVX(chip8 *chip, u8 x){

    u8 value = chip->V[x];
    u8 ones = value % 10;
    value /= 10;
    u8 tens = value % 10;
    u8 hundreds = value / 10;

    *(chip->ram + chip->I) = hundreds;
    *(chip->ram + (chip->I + 1)) = tens;
    *(chip->ram + (chip->I + 2)) = ones;

}

//Fx55 - LD [I], Vx
//LD [I], Vx
//The interpreter copies
// the values of registers V0 through Vx into memory,
// starting at the address in I.
//WORKS
void LDIVX(chip8 *chip, u8 x){
    for(int i = 0; i <= x; i++){
       *(chip->ram + (chip->I+i)) = chip->V[i];
    }
}

//Fx65 - LD Vx, [I]
// LD Vx, [I]
//The interpreter reads values from memory
// starting at location I into registers
// V0 through Vx.
//WORKS
void LDVXI(chip8 *chip, u8 x){
    for(int i = 0; i < x; i++){
        chip->V[i] = *(chip->ram + (chip->I + i));
    }

	chip->I += x + 1; //I's address is now the address
				      // after the end of the assignment
}

void UNKWN_Opcode(u16 Opcode){
    printf("Opcode: %04X\n", Opcode);
}

#pragma clang diagnostic pop
