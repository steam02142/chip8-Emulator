#pragma once
#include <iostream>
#include <cstdint>
#include <string.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <bits/stdc++.h>
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
};

int randomNumber();

