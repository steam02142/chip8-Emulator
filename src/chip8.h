#pragma once
#include <iostream>
#include <cstdint>
#include <string.h>
#include <fstream>
#include <filesystem>
#include <bitset>
#include <unordered_map>
#include "display.h"

using namespace std;

const int NumKeys = 16;

class Chip8{
    public:
        void loadProgram(string path);
        void fetch(uint16_t& opcode);
        void decode(uint16_t opcode, SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight]);
        void display(uint16_t opcode, bool display[DisplayWidth][DisplayHeight]);
        void handleInput(SDL_Event event);
        void storeInput(const char* key, bool value);
        uint8_t checkForKeypress();
        Chip8();

        uint8_t delay;
        uint8_t sound;
    private:
        // CPU memory
        uint8_t memory[4096];
        bool keysPressed[NumKeys];

        // Registers (V0 - VF)
        uint8_t V[16];

        uint16_t I;

        
        uint16_t stack[16];
        uint8_t stackPointer;
        uint16_t PC;

        void handleOpcode0(uint16_t opcode, SDL_Renderer* renderer, bool display[DisplayWidth][DisplayHeight]);
        void handleOpcode1(uint16_t opcode);
        void handleOpcode2(uint16_t opcode);
        void handleOpcode3(uint16_t opcode, uint16_t x);
        void handleOpcode4(uint16_t opcode, uint16_t x);
        void handleOpcode5(uint16_t opcode, uint16_t x, uint16_t y);
        void handleOpcode6(uint16_t opcode, uint16_t x);
        void handleOpcode7(uint16_t opcode, uint16_t x);
        void handleOpcode8(uint16_t opcode, uint16_t x, uint16_t y);
        void handleOpcode9(uint16_t opcode, uint16_t x, uint16_t y);
        void handleOpcodeA(uint16_t opcode);
        void handleOpcodeB(uint16_t opcode);
        void handleOpcodeC(uint16_t opcode, uint16_t x);
        void handleOpcodeD(uint16_t opcode, bool display[DisplayWidth][DisplayHeight]);
        void handleOpcodeE(uint16_t opcode, uint16_t x);
        void handleOpcodeF(uint16_t opcode, uint16_t x);

};

int randomNumber();

