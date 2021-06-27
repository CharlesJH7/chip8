 //
// Created by charl on 1/5/2020.
//

#ifndef CHIP8_INSTRUCTIONS_H
#define CHIP8_INSTRUCTIONS_H

#define BYTES_PER_CHAR_SPRITE 5


#include<stdlib.h>
#include<time.h>

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

void SysAddr(chip8 *chip, u16 nnn);

    /*
     *This instruction is only used on the old computers
     * on which Chip-8 was originally implemented.
     * It is ignored by modern interpreters.
     */

//00E0 - CLS
//Clear the Display

void CLS(chip8 *chip);

//00EE - RET
//Return from a subroutine

void RET(chip8 *chip);

//1nnn - JP addr
//Call addr

void JPaddr(chip8 *chip, u16 nnn);

//2nnn - CALL addr
//Call subroutine at nnn

void CALLaddr(chip8 *chip, u16 nnn);

//3xkk - SE Vx, byte
//Skip next instruction if Vx == kk
//WORKS
void SEVXbyte(chip8 *chip, u8 x, u8 kk);

//4xkk - SNE Vx, byte
//Skip next instruction if Vx != kk
//
void SNEVXbyte(chip8 *chip, u8 x, u8 kk);

//5xy0 - SE Vx, Vy
//Skip next instruction if Vx == Vy
//WORKS
void SEVXVY(chip8 *chip, u8 x, u8 y);

//6xkk - LD Vx, byte
//Set Vx = kk
//WORKS
void LDVXbyte(chip8 *chip, u8 x, u8 kk);

//7xkk - ADD Vx, byte
//Set Vx = Vx + kk
//WORKS
void ADDVXbyte(chip8 *chip, u8 x, u8 kk);

//8xy0 - LD Vx, Vy
//Set Vx = Vy

void LDVXVY(chip8 *chip, u8 x, u8 y);

//8xy1 - OR Vx, Vy
//Set Vx = Vx OR Vy
//WORKS
void ORVXVY(chip8 *chip, u8 x, u8 y);

//8xy2 - AND Vx, Vy
//Set Vx = Vx AND Vy
//WORKS
void ANDVXVY(chip8 *chip, u8 x, u8 y);


//8xy3 - XOR Vx, Vy
//Set Vx = Vx XOR Vy
//WORKS
void XORVXVY(chip8 *chip, u8 x, u8 y);

//8xy4 - ADD Vx, Vy
//Set Vx = Vx + Vy, set VF = carry
//WORKS
void ADDVXVY(chip8 *chip, u8 x, u8 y);

//8xy5 - SUB Vx, Vy
//Set Vx = Vx - Vy, set VF = NOT borrow
//WORKS
void SUBVXVY(chip8 *chip, u8 x, u8 y);

//8xy6 - SHR Vx {, Vy}
//Set Vx = Vx SHR 1
//WORKS
void SHRVX(chip8 *chip, u8 x);

//8xy7 - SUBN Vx, Vy
//Set Vx = Vy - Vx, set VF = NOT borrow

void SUBNVXVY(chip8 *chip, u8 x, u8 y);

//8xyE - SHL Vx {, Vy}
//Set Vx = Vx SHL 1

void SHLVX(chip8 *chip, u8 x);

//9xy0 - SNE Vx, Vy
//Skip next instruction if Vx != Vy

void SNEVXVY(chip8 *chip, u8 x, u8 y);

//Annn - LD I, addr
//Set I = nnn

void LDIaddr(chip8 *chip, u16 nnn);

//Bnnn - JP V0, addr
//Jump to location nnn + V0

void JMPnnnV0(chip8 *chip, u16 nnn);

//Cxkk - RND Vx, byte
//Set Vx = random byte AND kk

void RNDVXbyte(chip8 *chip, u8 x, u8 kk);

//Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision

//Pseudocode for DRAW instruction
//http://www.emulator101.com/chip-8-sprites.html
void DRWsprite(chip8 *chip, u8 x, u8 y, u8 n);

//Ex9E - SKP Vx
//Skip next instruction if key with the value of Vx is pressed

void SKPVX(chip8 *chip, u8 x);

//ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed

void SKNPVX(chip8 *chip, u8 x);

//Fx07 - LD Vx, DT
//Set Vx = delay timer value

void LDVXDT(chip8 *chip, u8 x);

//Fx0A - LD Vx, K
//Wait for a key press, store the value of the key in Vx

void LDVXK(chip8 *chip, u8 x);

//Fx15 - LD DT, Vx
//Set delay Timer = Vx

void LDDTVX(chip8 *chip, u8 x);

//Fx18 - LD ST, Vx
//Set sound timer = Vx

void LDSTVX(chip8 *chip, u8 x);

//Fx1E - ADD I, Vx
//Set I = I + Vx

void ADDIVX(chip8 *chip, u8 x);

//Fx29 - LD F, Vx
//Set I = location of sprite for digit Vx

void LDFVX(chip8 *chip, u8 x);

//Fx33 - LD B, Vx
//Store BCD representation of Vx in memory locations I, I+1, and I+2
//WORKS
void LDBVX(chip8 *chip, u8 x);

//Fx55 - LD [I], Vx
//LD [I], Vx

void LDIVX(chip8 *chip, u8 x);

//Fx65 - LD Vx, [I]
// LD Vx, [I]

void LDVXI(chip8 *chip, u8 x);


void UNKWN_Opcode(u16 Opcode);

#endif //CHIP8_INSTRUCTIONS_H


