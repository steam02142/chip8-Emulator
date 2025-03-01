#pragma once
#include <iostream>
#include <cstdint>
#include <string.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include "display.h"

using namespace std;

class Chip8{
    public:
        void loadProgram(string path);
        void fetch(uint16_t& opcode);
        void decode(uint16_t opcode, SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight]);
        void display(uint16_t opcode, bool display[DisplayWidth][DisplayHeight]);
        Chip8();
    private:
        // CPU memory
        uint8_t memory[4096];

        // Registers (V0 - VF)
        uint8_t V[16];

        uint16_t I;
        uint8_t delay;
        uint8_t sound;
        
        uint16_t stack[16];
        uint8_t stackPointer;
        uint16_t PC;
};