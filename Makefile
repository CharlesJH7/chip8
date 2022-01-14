CC=gcc

SRC = $(wildcard *.c)
SDLFlags = -lmingw32 -lSDL2main -lSDL2


all:
	GCC -std=c99 *.c -o chip8 $(SDLFlags)

test1:
	./chip8.exe BC_test.ch8 

test2:
	./chip8.exe test_opcode.ch8

run:
	./chip8.exe INVADERS

clean:
	rm *.o *.un~







